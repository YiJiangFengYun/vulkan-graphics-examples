#include "graphics_ext/material/material_deferred.hpp"

namespace vge
{
    MaterialDeferred::DeferredAttachmentInfo::DeferredAttachmentInfo(vk::Format format)
        : format(format)
    {}

    MaterialDeferred::CreateInfo::CreateInfo(uint32_t framebufferWidth
        , uint32_t framebufferHeight
        , vk::Format colorAttachmentFormat
        , vk::Format depthStencilAttachmentFormat
        , uint32_t deferredAttachmentCount
        , const DeferredAttachmentInfo *pDeferredAttachments
        )
        : framebufferWidth(framebufferWidth)
        , framebufferHeight(framebufferHeight)
        , colorAttachmentFormat(colorAttachmentFormat)
        , depthStencilAttachmentFormat(depthStencilAttachmentFormat)
        , deferredAttachmentCount(deferredAttachmentCount)
        , pDeferredAttachments(pDeferredAttachments)
    {}

    MaterialDeferred::MaterialDeferred(CreateInfo info)
        : vg::Material(VG_TRUE)
        , m_info(info)
        , m_deferredAttachmentInfos()
        , m_pColorAttachment()
        , m_pDepthStencilAttachment()
        , m_arrPDeferredAttachments()
        , m_pRectMesh(std::shared_ptr<vg::DimSepMesh2>{
                new vg::DimSepMesh2(vk::MemoryPropertyFlagBits::eHostVisible)
            })
    {
        m_deferredAttachmentInfos.resize(info.deferredAttachmentCount);
        memcpy(m_deferredAttachmentInfos.data(), 
            info.pDeferredAttachments, 
            info.deferredAttachmentCount * sizeof(DeferredAttachmentInfo));
        m_info.pDeferredAttachments = m_deferredAttachmentInfos.data();

        _createDeferredAttachments(info);
        _createRenderPass(info);
        _createFramebuffer();
        _createOtherPasses(info);
        _initPasses(info);
    }

    void MaterialDeferred::_beginBind(const BindInfo info, BindResult *pResult) const
    {
        uint32_t trunkFramebufferWidth = info.trunkFramebufferWidth;
        uint32_t trunkFramebufferHeight = info.trunkFramebufferHeight;
        uint32_t framebufferWidth = m_info.framebufferWidth;
        uint32_t framebufferHeight = m_info.framebufferHeight;

        uint32_t targetSize = static_cast<uint32_t>(
            std::max(
                info.clipRect.width * trunkFramebufferWidth, 
                info.clipRect.height * trunkFramebufferHeight
            )
        );

        auto pRectMesh = _getRectMesh();
        //rect pos
        std::vector<vg::Vector2> rectPoses(4);
        //rect uv
        std::vector<vg::Vector2> rectUVs(4);
        fd::Viewport viewport;
        fd::Rect2D scissor;
        if (info.hasClipRect) {
            // in this case, we only draw the part of scene that contain the object of the mesh.
            auto rateX = static_cast<float>(trunkFramebufferWidth) / static_cast<float>(framebufferWidth);
            auto rateY = static_cast<float>(trunkFramebufferHeight) / static_cast<float>(framebufferHeight);
            viewport = fd::Viewport(
                rateX * (- info.clipRect.x), 
                rateY * (- info.clipRect.y), 
                rateX * 1.0f,
                rateY * 1.0f
            );
            
            scissor = fd::Rect2D(0.0f, 0.0f, rateX * info.clipRect.width, rateY * info.clipRect.height);
            rectPoses = {
                vg::Vector2{info.clipRect.x,                       info.clipRect.y},
                vg::Vector2{info.clipRect.x + info.clipRect.width, info.clipRect.y},
                vg::Vector2{info.clipRect.x + info.clipRect.width, info.clipRect.y + info.clipRect.height},
                vg::Vector2{info.clipRect.x,                       info.clipRect.y + info.clipRect.height},
            };
            rectUVs = {
                vg::Vector2{0.0f, 0.0f},
                vg::Vector2{rateX * info.clipRect.width, 0.0f},
                vg::Vector2{rateX * info.clipRect.width, rateY * info.clipRect.height},
                vg::Vector2{0.0f, rateY * info.clipRect.height},
            };
        } else {
            // in this case, we will cast entire scene where there is the object of the mesh in.
            viewport = fd::Viewport();
            scissor = info.hasClipRect ? info.clipRect : fd::Rect2D();
            rectPoses = {
                vg::Vector2{0.0f, 0.0f},
                vg::Vector2{1.0f, 0.0f},
                vg::Vector2{1.0f, 1.0f},
                vg::Vector2{0.0f, 1.0f},
            };
            rectUVs = {
                vg::Vector2{0.0f, 0.0f},
                vg::Vector2{1.0f, 0.0f},
                vg::Vector2{1.0f, 1.0f},
                vg::Vector2{0.0f, 1.0f},
            };
        }
        //map [0, 1] to [-1, 1];
        for (auto &point : rectPoses) {
            point.x = point.x * 2 - 1;
            point.y = point.y * 2 - 1;
        }
    
        std::vector<uint32_t> indices = {
            0, 1, 3, 3, 1, 2
        };
    
        pRectMesh->setVertexCount(static_cast<uint32_t>(rectPoses.size()));
        if (m_pRectMesh->hasPositions() == VG_TRUE)
        {
            m_pRectMesh->removePositions();
        }
        pRectMesh->addPositions(rectPoses);
        if (m_pRectMesh->hasTextureCoordinates<vg::TextureCoordinateType::VECTOR_2, vg::TextureCoordinateIndex::TextureCoordinate_0>() == VG_TRUE)
        {
            m_pRectMesh->removeTextureCoordinates<vg::TextureCoordinateType::VECTOR_2, vg::TextureCoordinateIndex::TextureCoordinate_0>();
        }
        pRectMesh->addTextureCoordinates<vg::TextureCoordinateType::VECTOR_2, vg::TextureCoordinateIndex::TextureCoordinate_0>(
            rectUVs
        );
        pRectMesh->setIndices(indices, vg::PrimitiveTopology::TRIANGLE_LIST, 0u);
        pRectMesh->apply(VG_TRUE);
    
        auto &result = *pResult;

        //begin render pass.
        {
            vg::RenderPassBeginInfo renderPassBeginInfo;
            renderPassBeginInfo.pRenderPass = m_pRenderPass.get();
            renderPassBeginInfo.pFramebuffer = m_pFrameBuffer.get();
            renderPassBeginInfo.framebufferWidth = framebufferWidth;
            renderPassBeginInfo.framebufferHeight = framebufferHeight;
            renderPassBeginInfo.renderArea = fd::Rect2D();

            vk::ClearValue clearValueColor = {
                std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f}
            };
            vk::ClearValue clearValueDepthStencil = {
                vk::ClearDepthStencilValue(1.0f, 0)
            };

            vk::ClearValue clearValues[5];
            clearValues[0] = clearValueColor;
            clearValues[1] = clearValueColor;
            clearValues[2] = clearValueColor;
            clearValues[3] = clearValueColor;
            clearValues[4] = clearValueDepthStencil;
    
            renderPassBeginInfo.clearValueCount = 5;
            renderPassBeginInfo.pClearValues = clearValues;

            vg::CmdInfo cmdInfo;
            cmdInfo.pRenderPassBeginInfo = &renderPassBeginInfo;
            result.pBranchCmdBuffer->addCmd(cmdInfo);
        }
    
        //frist pass
        {
            vg::RenderPassInfo renderPassInfo;
            renderPassInfo.pRenderPass = m_pRenderPass.get();
            renderPassInfo.subPassIndex = 0u;
            renderPassInfo.pFramebuffer = m_pFrameBuffer.get();
            renderPassInfo.framebufferWidth = framebufferWidth;
            renderPassInfo.framebufferHeight = framebufferHeight;
            renderPassInfo.projMatrix = *(info.pProjMatrix);
            renderPassInfo.viewMatrix = *(info.pViewMatrix);
            renderPassInfo.pPass = m_pPassDeferred.get();
            renderPassInfo.modelMatrix = *(info.pModelMatrix);
            renderPassInfo.pMesh = info.pMesh;
            renderPassInfo.subMeshIndex = info.subMeshIndex;
            renderPassInfo.viewport = viewport;
            renderPassInfo.scissor = scissor;
    
            vg::CmdInfo cmdInfo;
            cmdInfo.pRenderPassInfo = &renderPassInfo;
            result.pBranchCmdBuffer->addCmd(cmdInfo);
        }
    
        {
            
            vg::RenderPassInfo renderPassInfo;
            renderPassInfo.pRenderPass = m_pRenderPass.get();
            renderPassInfo.subPassIndex = 1u;
            renderPassInfo.pFramebuffer = m_pFrameBuffer.get();
            renderPassInfo.framebufferWidth = framebufferWidth;
            renderPassInfo.framebufferHeight = framebufferHeight;
            renderPassInfo.projMatrix = vg::Matrix4x4(1.0f);
            renderPassInfo.viewMatrix = vg::Matrix4x4(1.0f);
            renderPassInfo.pPass = m_pPassComposition.get();
            renderPassInfo.modelMatrix = vg::Matrix4x4(1.0f);
            renderPassInfo.pMesh = nullptr;
            renderPassInfo.subMeshIndex = 0u;
            renderPassInfo.viewport = fd::Viewport();
            renderPassInfo.scissor = scissor;
    
            vg::CmdDraw cmdDraw = {3,1,0,0};
            renderPassInfo.pCmdDraw = &cmdDraw;
    
            vg::CmdInfo cmdInfo;
            cmdInfo.pRenderPassInfo = &renderPassInfo;
            result.pBranchCmdBuffer->addCmd(cmdInfo);
    
        }

        {
            vg::RenderPassEndInfo renderPassEndInfo;
            vg::CmdInfo cmdInfo;
            cmdInfo.pRenderPassEndInfo = &renderPassEndInfo;
            result.pBranchCmdBuffer->addCmd(cmdInfo);
        }
    
        //final pass is trunk pass.
        {
            vg::RenderPassInfo trunkRenderPassInfo;
            trunkRenderPassInfo.pRenderPass = nullptr;
            trunkRenderPassInfo.pFramebuffer = nullptr;
            trunkRenderPassInfo.framebufferWidth = info.trunkFramebufferWidth;
            trunkRenderPassInfo.framebufferHeight = info.trunkFramebufferHeight;
            trunkRenderPassInfo.projMatrix = vg::Matrix4x4(1.0f);
            trunkRenderPassInfo.viewMatrix = vg::Matrix4x4(1.0f);
            trunkRenderPassInfo.pPass = m_pMainPass.get();
            trunkRenderPassInfo.modelMatrix = vg::Matrix4x4(1.0f);
            trunkRenderPassInfo.pMesh = pRectMesh;
            trunkRenderPassInfo.subMeshIndex = 0u;
            trunkRenderPassInfo.viewport = fd::Viewport();
            trunkRenderPassInfo.scissor = info.hasClipRect ? info.clipRect : fd::Rect2D();
    
            vg::CmdInfo cmdInfo;
            cmdInfo.pRenderPassInfo = &trunkRenderPassInfo;
            result.pTrunkRenderPassCmdBuffer->addCmd(cmdInfo);
        }
    
    }

    vg::Pass * MaterialDeferred::getPassDeferred() const
    {
        return m_pPassDeferred.get();
    }
    
    vg::Pass * MaterialDeferred::getPassComposition() const
    {
        return m_pPassComposition.get();
    }

    void MaterialDeferred::_createDeferredAttachments(CreateInfo createInfo)
    {
        m_arrPDeferredAttachments.resize(createInfo.deferredAttachmentCount);
        {
            for (uint32_t i = 0; i < createInfo.deferredAttachmentCount; ++i) {
                auto deferredAttachmentInfo = *(createInfo.pDeferredAttachments + i);
                m_arrPDeferredAttachments[i] = std::shared_ptr<vg::Texture2DColorAttachment>{
                    new vg::Texture2DColorAttachment(deferredAttachmentInfo.format, createInfo.framebufferWidth,
                        createInfo.framebufferHeight, VG_TRUE)
                };
            }
        }
        
        {
            m_pColorAttachment = std::shared_ptr<vg::Texture2DColorAttachment>{
                new vg::Texture2DColorAttachment(createInfo.colorAttachmentFormat, createInfo.framebufferWidth,
                    createInfo.framebufferHeight)
            };
        }

        {
            m_pDepthStencilAttachment = std::shared_ptr<vg::Texture2DDepthStencilAttachment>{
                new vg::Texture2DDepthStencilAttachment(createInfo.depthStencilAttachmentFormat,
                    createInfo.framebufferWidth, createInfo.framebufferHeight, VG_FALSE, vk::ImageUsageFlags()
                )
            };
            //Create depth only image view for sampling from shader in trunk pass.
            auto pImage = m_pDepthStencilAttachment->getImage();
            vg::Texture::ImageViewCreateInfo info = {
                m_pDepthStencilAttachment->getImageViewType(),
                vk::ComponentMapping(),
                {
                    vk::ImageAspectFlagBits::eDepth,
                    0u,
                    pImage->getInfo().mipLevels,
                    0u,
                    pImage->getInfo().arrayLayers,
                },
            };
            m_pDepthStencilAttachment->createImageView("only_depth", info);
        }
    }
        
    void MaterialDeferred::_createRenderPass(CreateInfo createInfo)
    {
        uint32_t attachmentCount = createInfo.deferredAttachmentCount + 2;
        std::vector<vk::AttachmentDescription> attachmentDescs(attachmentCount);
        std::vector<vk::Format> attachmentFormats(attachmentCount);
        std::vector<vk::ImageLayout> attachmentLayouts(attachmentCount);
        uint32_t offset = 0u;
        attachmentFormats[offset] = createInfo.colorAttachmentFormat;
        attachmentLayouts[offset] = vk::ImageLayout::eShaderReadOnlyOptimal;
        ++offset;
        for (uint32_t i = 0; i < attachmentCount - 2; ++i, ++offset)
        {
            attachmentFormats[offset] = (createInfo.pDeferredAttachments + i)->format;
            attachmentLayouts[offset] = vk::ImageLayout::eShaderReadOnlyOptimal;
        }
        attachmentFormats[offset] = createInfo.depthStencilAttachmentFormat;
        attachmentLayouts[offset] = vk::ImageLayout::eDepthStencilReadOnlyOptimal;

        for (uint32_t i = 0; i < attachmentCount; ++i) {
            attachmentDescs[i].format = attachmentFormats[i];
            attachmentDescs[i].samples = vk::SampleCountFlagBits::e1;
            attachmentDescs[i].loadOp = vk::AttachmentLoadOp::eClear;
            attachmentDescs[i].storeOp = vk::AttachmentStoreOp::eDontCare;
            attachmentDescs[i].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
            attachmentDescs[i].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
            attachmentDescs[i].initialLayout = attachmentLayouts[i];
            attachmentDescs[i].finalLayout = attachmentLayouts[i];
        }
    
        attachmentDescs[0].storeOp = vk::AttachmentStoreOp::eStore;
        attachmentDescs[attachmentCount - 1].storeOp = vk::AttachmentStoreOp::eStore;
    
        uint32_t subpassCount = 2u;
        std::vector<vk::SubpassDescription> subpassDescs(subpassCount);
    
        
        //first subpass: fill G-Buffer components
        std::vector<vk::AttachmentReference> colorAttachmentRefs1(createInfo.deferredAttachmentCount);
        for (uint32_t i = 0; i < createInfo.deferredAttachmentCount; ++i)
        {
            colorAttachmentRefs1[i] = {i + 1, vk::ImageLayout::eColorAttachmentOptimal};
        }

        vk::AttachmentReference depthAttachmentRef1 = {attachmentCount - 1, vk::ImageLayout::eDepthStencilAttachmentOptimal};

        subpassDescs[0].pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        subpassDescs[0].colorAttachmentCount = createInfo.deferredAttachmentCount;
        subpassDescs[0].pColorAttachments = colorAttachmentRefs1.data();
        subpassDescs[0].pDepthStencilAttachment = &depthAttachmentRef1;
        
    
        
        //Second subpass: final composition with G-Buffer components.
        vk::AttachmentReference colorAttachmentRef2 = {0, vk::ImageLayout::eColorAttachmentOptimal};
        std::vector<vk::AttachmentReference> inputAttachmentRefs2(createInfo.deferredAttachmentCount);
        for (uint32_t i = 0; i < createInfo.deferredAttachmentCount; ++i)
        {
            inputAttachmentRefs2[i] = {i + 1, vk::ImageLayout::eShaderReadOnlyOptimal};
        } 
        subpassDescs[1].pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        subpassDescs[1].colorAttachmentCount = 1u;
        subpassDescs[1].pColorAttachments = &colorAttachmentRef2;
        subpassDescs[1].pDepthStencilAttachment = nullptr;;
        subpassDescs[1].inputAttachmentCount = static_cast<uint32_t>(inputAttachmentRefs2.size());
        subpassDescs[1].pInputAttachments = inputAttachmentRefs2.data();
        
    
        uint32_t dependencyCount = subpassCount + 1;
        std::vector<vk::SubpassDependency> dependencies(dependencyCount);
    
        dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
        dependencies[0].dstSubpass = 0;
        dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eTopOfPipe;
        dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eLateFragmentTests;
        dependencies[0].srcAccessMask = vk::AccessFlagBits::eShaderRead;
        dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite |
            vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
        dependencies[0].dependencyFlags = vk::DependencyFlagBits::eByRegion;
    
        dependencies[1].srcSubpass = 0;
        dependencies[1].dstSubpass = 1;
        dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eLateFragmentTests;
        dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eFragmentShader;
        dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite |
            vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
        dependencies[1].dstAccessMask = vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
        dependencies[1].dependencyFlags = vk::DependencyFlagBits::eByRegion;
    
        dependencies[2].srcSubpass = 1;
        dependencies[2].dstSubpass = VK_SUBPASS_EXTERNAL;
        dependencies[2].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependencies[2].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
        dependencies[2].srcAccessMask = vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
        dependencies[2].dstAccessMask = vk::AccessFlagBits::eShaderRead;
        dependencies[2].dependencyFlags = vk::DependencyFlagBits::eByRegion;
        
    
        vk::RenderPassCreateInfo renderPassCreateInfo = {
            vk::RenderPassCreateFlags(),
            static_cast<uint32_t>(attachmentDescs.size()),
            attachmentDescs.data(),
            static_cast<uint32_t>(subpassDescs.size()),
            subpassDescs.data(),
            static_cast<uint32_t>(dependencies.size()),
            dependencies.data()
        };
        auto pDevice = vg::pApp->getDevice();
        m_pRenderPass = fd::createRenderPass(pDevice, renderPassCreateInfo);
    }

    void MaterialDeferred::_createFramebuffer()
    {
        auto info = m_info;
        uint32_t attachmentCount = info.deferredAttachmentCount;
        attachmentCount += 2u;

        std::vector<vk::ImageView> attachments(attachmentCount);
        uint32_t offset = 0u;
        attachments[offset] = *(m_pColorAttachment->getImageView()->getImageView());
        ++offset;
        for (uint32_t i = 0; i < info.deferredAttachmentCount; ++i, ++offset)
        {
            attachments[offset] = *(m_arrPDeferredAttachments[i]->getImageView()->getImageView());
        }
        attachments[offset] = *(m_pDepthStencilAttachment->getImageView()->getImageView());
    
        vk::FramebufferCreateInfo createInfo = {
            vk::FramebufferCreateFlags(),                   //flags
            *m_pRenderPass,                                 //renderPass
            attachmentCount,                                //attachmentCount
            attachments.data(),                             //pAttachments
            info.framebufferWidth,                               //width
            info.framebufferHeight,                               //height
            1u                                              //layers
        };
        auto pDevice = vg::pApp->getDevice();
        m_pFrameBuffer = fd::createFrameBuffer(pDevice, createInfo);
    }

    void MaterialDeferred::_createOtherPasses(CreateInfo createInfo)
    {
        m_pShaderDeferred = std::shared_ptr<vg::Shader>{new vg::Shader()};
        m_pShaderComposition = std::shared_ptr<vg::Shader>{new vg::Shader()};
        m_pPassDeferred = std::shared_ptr<vg::Pass>{ new vg::Pass(m_pShaderDeferred.get())};
        m_pPassComposition = std::shared_ptr<vg::Pass>{ new vg::Pass(m_pShaderComposition.get())};
        _addPass(m_pPassDeferred.get());
        _addPass(m_pPassComposition.get());


    }
        
    void MaterialDeferred::_initPasses(CreateInfo createInfo)
    {
         //deferred pass
        {
            auto pPass = m_pPassDeferred.get();
            pPass->setSubpass(0u);
            const uint32_t attachmentCount = 3u;
            vk::PipelineColorBlendAttachmentState attachmentStates[attachmentCount] = {};
            for (uint32_t i = 0; i < attachmentCount; ++i)
            {
                attachmentStates[i].colorWriteMask = vk::ColorComponentFlagBits::eR | 
                    vk::ColorComponentFlagBits::eG | 
                    vk::ColorComponentFlagBits::eB | 
                    vk::ColorComponentFlagBits::eA;
                attachmentStates[i].blendEnable = VG_FALSE;
            }
            
            vk::PipelineColorBlendStateCreateInfo colorBlendState = {};
            colorBlendState.attachmentCount = attachmentCount;
            colorBlendState.pAttachments = attachmentStates;
            pPass->setColorBlendInfo(colorBlendState);
            vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
            depthStencilState.depthTestEnable = VG_TRUE;
            depthStencilState.depthWriteEnable = VG_TRUE;
            depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
            pPass->setDepthStencilInfo(depthStencilState);
            pPass->apply();
        }
    
        //composition pass
        {
            auto pPass = m_pPassComposition.get();
            pPass->setSubpass(1u);
            pPass->setPolygonMode(vk::PolygonMode::eFill);
            pPass->setCullMode(vk::CullModeFlagBits::eNone);
            pPass->setFrontFace(vk::FrontFace::eCounterClockwise);
            vg::Pass::BuildInDataInfo buildInDataInfo;
            buildInDataInfo.componentCount = 0u;
            buildInDataInfo.pComponent = nullptr;
            pPass->setBuildInDataInfo(buildInDataInfo);
    
            vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState = {
                vk::PipelineInputAssemblyStateCreateFlags(),
                vk::PrimitiveTopology::eTriangleList
            };
    
            pPass->setDefaultInputAssemblyState(inputAssemblyState);
            
             const uint32_t attachmentCount = 1u;
            vk::PipelineColorBlendAttachmentState attachmentStates[attachmentCount] = {};
            for (uint32_t i = 0; i < attachmentCount; ++i)
            {
                attachmentStates[i].colorWriteMask = vk::ColorComponentFlagBits::eR | 
                    vk::ColorComponentFlagBits::eG | 
                    vk::ColorComponentFlagBits::eB | 
                    vk::ColorComponentFlagBits::eA;
                attachmentStates[i].blendEnable = VG_FALSE;
            }
            
            vk::PipelineColorBlendStateCreateInfo colorBlendState = {};
            colorBlendState.attachmentCount = attachmentCount;
            colorBlendState.pAttachments = attachmentStates;
            pPass->setColorBlendInfo(colorBlendState);
    
            vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
            depthStencilState.depthTestEnable = VG_FALSE;
            depthStencilState.depthWriteEnable = VG_FALSE;
            depthStencilState.depthCompareOp = vk::CompareOp::eAlways;
            pPass->setDepthStencilInfo(depthStencilState);

            
                
            pPass->apply();
        }
    
        //main pass
        {
            auto pPass = m_pMainPass.get();
            pPass->setPolygonMode(vk::PolygonMode::eFill);
            pPass->setCullMode(vk::CullModeFlagBits::eNone);
            pPass->setFrontFace(vk::FrontFace::eCounterClockwise);
            vg::Pass::BuildInDataInfo buildInDataInfo;
            buildInDataInfo.componentCount = 0u;
            buildInDataInfo.pComponent = nullptr;
            pPass->setBuildInDataInfo(buildInDataInfo);
            vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
            depthStencilState.depthTestEnable = VG_TRUE;
            depthStencilState.depthWriteEnable = VG_TRUE;
            depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
            pPass->setDepthStencilInfo(depthStencilState);
            
            pPass->apply();
        }

        {
            auto pPass = m_pPassComposition.get();
            auto info = m_info;
            for (uint32_t i = 0; i < info.deferredAttachmentCount; ++i) 
            {
                vg::PassTextureInfo::TextureInfo itemInfo = {
                    m_arrPDeferredAttachments[i].get(),
                    nullptr,
                    nullptr,
                    vk::ImageLayout::eUndefined,
                };
                vg::PassTextureInfo info = {
                    vg::SamplerTextureType::TEX_2D,
                    1u,
                    &itemInfo,
                    i + 1, //0 is own by build in buffer data of pass.
                    vg::ImageDescriptorType::INPUT_ATTACHMENT,
                    vk::ShaderStageFlagBits::eFragment,
                };
                pPass->addTexture("input_" + std::to_string(i), info);
            }
            pPass->apply();
        }

        {
            auto pPass = m_pMainPass.get();
            {
                vg::PassTextureInfo::TextureInfo itemInfo = {
                    m_pColorAttachment.get(),
                    nullptr,
                    nullptr,
                    vk::ImageLayout::eUndefined,
                };
                vg::PassTextureInfo info = {
                    vg::SamplerTextureType::TEX_2D,
                    1u,
                    &itemInfo,
                    0u,
                    vg::ImageDescriptorType::COMBINED_IMAGE_SAMPLER,
                    vk::ShaderStageFlagBits::eFragment,
                };
                pPass->addTexture("color", info);
            }

            {
                vg::PassTextureInfo::TextureInfo itemInfo = {
                    m_pDepthStencilAttachment.get(),
                    m_pDepthStencilAttachment->getImageView("only_depth"),
                    nullptr,
                    vk::ImageLayout::eUndefined,
                };
                vg::PassTextureInfo info = {
                    vg::SamplerTextureType::TEX_2D,
                    1u,
                    &itemInfo,
                    1u,
                    vg::ImageDescriptorType::COMBINED_IMAGE_SAMPLER,
                    vk::ShaderStageFlagBits::eFragment,
                };
                pPass->addTexture("depth", info);
            }
            pPass->apply();
        }
    }

    vg::DimSepMesh2 *MaterialDeferred::_getRectMesh() const
    {
        return m_pRectMesh.get();
    }

} //vge