#include "passes/material_deferred.hpp"

MaterialDeferred::MaterialDeferred()
    : vg::Material()
    , m_frameBufferWidth(1024)
    , m_frameBufferHeight(1024)
    , m_trunkFramebufferWidth(0u)
    , m_trunkFramebufferHeight(0u)
    , m_mapRectMeshes()
{
    _createAttachments();
    _createRenderPass();
    _createFramebuffer();
    _createOtherPasses();
    _initPasses();
}

MaterialDeferred::MaterialDeferred(uint32_t trunkFrameBufferWidth, uint32_t trunkFrameBufferHeight)
    : vg::Material()
    , m_frameBufferWidth(1024)
    , m_frameBufferHeight(1024)
    , m_trunkFramebufferWidth(trunkFrameBufferWidth)
    , m_trunkFramebufferHeight(trunkFrameBufferHeight)
    , m_mapRectMeshes()
{
    _createAttachments();
    _createRenderPass();
    _createFramebuffer();
    _createOtherPasses();
    _initPasses();
}

void MaterialDeferred::beginBindToRender(const BindInfo info, BindResult *pResult)
{

    auto pRectMesh = _getRectMesh(info.objectID);
    //rect pos
    std::vector<vg::Vector2> rectPoses(4);
    //rect uv
    std::vector<vg::Vector2> rectUVs(4);
    fd::Viewport viewport;
    fd::Rect2D scissor;
    if (info.hasClipRect && m_trunkFramebufferWidth != 0u && m_trunkFramebufferHeight != 0u) {
        // in this case, we only draw the part of scene that contain the object of the mesh.
        auto rateX = static_cast<float>(m_trunkFramebufferWidth) / static_cast<float>(m_frameBufferWidth);
        auto rateY = static_cast<float>(m_trunkFramebufferHeight) / static_cast<float>(m_frameBufferHeight);
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
    pRectMesh->setPositions(rectPoses);
    pRectMesh->setTextureCoordinates<vg::TextureCoordinateType::VECTOR_2, vg::TextureCoordinateIndex::TextureCoordinate_0>(
        rectUVs
    );
    pRectMesh->setIndices(indices, vg::PrimitiveTopology::TRIANGLE_LIST, 0u);
    pRectMesh->apply(VG_TRUE);

    auto &result = *pResult;

    //frist pass
    {
        vg::RenderPassInfo renderPassInfo;
        renderPassInfo.pRenderPass = m_pRenderPass.get();
        renderPassInfo.subPassIndex = 0u;
		renderPassInfo.pFrameBuffer = m_pFrameBuffer.get();
        renderPassInfo.framebufferWidth = m_frameBufferWidth;
        renderPassInfo.framebufferHeight = m_frameBufferHeight;
        renderPassInfo.projMatrix = *(info.pProjMatrix);
        renderPassInfo.viewMatrix = *(info.pViewMatrix);
        renderPassInfo.pPass = m_pPassDeferred.get();
        renderPassInfo.modelMatrix = *(info.pModelMatrix);
        renderPassInfo.pMesh = info.pMesh;
        renderPassInfo.subMeshIndex = info.subMeshIndex;
        renderPassInfo.viewport = viewport;
        renderPassInfo.scissor = scissor;

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

        renderPassInfo.clearValueCount = 5;
        renderPassInfo.pClearValues = clearValues;

        vg::CmdInfo cmdInfo;
        cmdInfo.pRenderPassInfo = &renderPassInfo;
        result.pBranchCmdBuffer->addCmd(cmdInfo);
    }

    {
        
        vg::RenderPassInfo renderPassInfo;
        renderPassInfo.pRenderPass = m_pRenderPass.get();
        renderPassInfo.subPassIndex = 1u;
		renderPassInfo.pFrameBuffer = m_pFrameBuffer.get();
        renderPassInfo.framebufferWidth = m_frameBufferWidth;
        renderPassInfo.framebufferHeight = m_frameBufferHeight;
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

    //trunk barrier for trunk pass sampling from attachment textures.
    {
        vk::ImageMemoryBarrier imageMemoryBarriers[2];

        //color texture.
        auto pImage = m_pAttachmentColor->getImage();
        imageMemoryBarriers[0].srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
        imageMemoryBarriers[0].dstAccessMask = vk::AccessFlagBits::eShaderRead;
        imageMemoryBarriers[0].oldLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
        imageMemoryBarriers[0].newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
        imageMemoryBarriers[0].subresourceRange = {
            pImage->getInfo().allAspect,
            0u,
            pImage->getInfo().mipLevels,
            0u,
            pImage->getInfo().arrayLayers,
        };
        imageMemoryBarriers[0].image = *(pImage->getImage());

        //depth texture.
        pImage = m_pAttachmentDepthStencil->getImage();
        imageMemoryBarriers[1].srcAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentWrite;
        imageMemoryBarriers[1].dstAccessMask = vk::AccessFlagBits::eShaderRead;
        imageMemoryBarriers[1].oldLayout = vk::ImageLayout::eDepthStencilReadOnlyOptimal;
        imageMemoryBarriers[1].newLayout = vk::ImageLayout::eDepthStencilReadOnlyOptimal;
        imageMemoryBarriers[1].subresourceRange = {
            pImage->getInfo().allAspect,
            0u,
            pImage->getInfo().mipLevels,
            0u,
            pImage->getInfo().arrayLayers,
        };
        imageMemoryBarriers[1].image = *(pImage->getImage());

        vg::BarrierInfo barrierInfo;
        barrierInfo.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        barrierInfo.dstStageMask = vk::PipelineStageFlagBits::eFragmentShader;
        barrierInfo.imageMemoryBarrierCount = 2;
        barrierInfo.pImageMemoryBarriers = imageMemoryBarriers;
        barrierInfo.dependencyFlags = vk::DependencyFlagBits::eByRegion;
        
        vg::CmdInfo cmdInfo;
        cmdInfo.pBarrierInfo = &barrierInfo;
        result.pTrunkWaitBarrierCmdBuffer->addCmd(cmdInfo);
    }

    //final pass is trunk pass.
    {
        vg::RenderPassInfo trunkRenderPassInfo;
        trunkRenderPassInfo.pRenderPass = nullptr;
		trunkRenderPassInfo.pFrameBuffer = nullptr;
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

void MaterialDeferred::endBindToRender(const EndBindInfo info)
{

}

vg::Pass * MaterialDeferred::getPassDeferred() const
{
    return m_pPassDeferred.get();
}

vg::Pass * MaterialDeferred::getPassComposition() const
{
    return m_pPassComposition.get();
}

void MaterialDeferred::_createAttachments()
{
    m_pAttachmentColor = std::shared_ptr<vg::Texture2DColorAttachment>{
        new vg::Texture2DColorAttachment(vk::Format::eR8G8B8A8Unorm, m_frameBufferWidth, m_frameBufferHeight)
    };

    m_pAttachmentPos = std::shared_ptr<vg::Texture2DColorAttachment>{
        new vg::Texture2DColorAttachment(vk::Format::eR16G16B16A16Sfloat, m_frameBufferWidth, m_frameBufferHeight, VG_TRUE)
    };
    
    m_pAttachmentNormal = std::shared_ptr<vg::Texture2DColorAttachment>{
       new vg::Texture2DColorAttachment(vk::Format::eR16G16B16A16Sfloat, m_frameBufferWidth, m_frameBufferHeight, VG_TRUE)
    };
    
    m_pAttachmentAlbedo = std::shared_ptr<vg::Texture2DColorAttachment>{
       new vg::Texture2DColorAttachment(vk::Format::eR8G8B8A8Unorm, m_frameBufferWidth, m_frameBufferHeight, VG_TRUE)
    };

    m_pAttachmentDepthStencil = std::shared_ptr<vg::Texture2DDepthStencilAttachment>{
       new vg::Texture2DDepthStencilAttachment(vk::Format::eD32SfloatS8Uint, m_frameBufferWidth, m_frameBufferHeight)
    };

    //Create depth only image view for sampling from shader in trunk pass.
    auto pImage = m_pAttachmentDepthStencil->getImage();
    vg::Texture::ImageViewCreateInfo info = {
        vk::ComponentMapping(),
        {
            vk::ImageAspectFlagBits::eDepth,
            0u,
            pImage->getInfo().mipLevels,
            0u,
            pImage->getInfo().arrayLayers,
        },
    };
    m_pAttachmentDepthStencil->createImageView("only_depth", info);
}

void MaterialDeferred::_createRenderPass()
{
    std::array<vk::AttachmentDescription, 5> attachmentDescs;
    std::array<vk::Format, 5> attachmentFormats = {
        m_pAttachmentColor->getImage()->getInfo().format,
        m_pAttachmentPos->getImage()->getInfo().format,
        m_pAttachmentNormal->getImage()->getInfo().format,
        m_pAttachmentAlbedo->getImage()->getInfo().format,
        m_pAttachmentDepthStencil->getImage()->getInfo().format,
    };
    std::array<vk::ImageLayout, 5> attachmentLayouts = {
        m_pAttachmentColor->getImage()->getInfo().layout,
        m_pAttachmentPos->getImage()->getInfo().layout,
        m_pAttachmentNormal->getImage()->getInfo().layout,
        m_pAttachmentAlbedo->getImage()->getInfo().layout,
        m_pAttachmentDepthStencil->getImage()->getInfo().layout,
    };
    for (uint32_t i = 0; i < 5; ++i) {
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
    attachmentDescs[4].storeOp = vk::AttachmentStoreOp::eStore;

    std::array<vk::SubpassDescription, 2> subpassDescs;

    {
        //first subpass: fill G-Buffer components
        std::array<vk::AttachmentReference, 4> attachmentRefs = {
            vk::AttachmentReference{0, vk::ImageLayout::eColorAttachmentOptimal},
            vk::AttachmentReference{1, vk::ImageLayout::eColorAttachmentOptimal},
            vk::AttachmentReference{2, vk::ImageLayout::eColorAttachmentOptimal},
            vk::AttachmentReference{3, vk::ImageLayout::eColorAttachmentOptimal},            
        };

        vk::AttachmentReference depthAttachmentRef = {4, vk::ImageLayout::eDepthStencilAttachmentOptimal};

        subpassDescs[0].pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        subpassDescs[0].colorAttachmentCount = 4;
        subpassDescs[0].pColorAttachments = attachmentRefs.data();
        subpassDescs[0].pDepthStencilAttachment = &depthAttachmentRef;
    }

    {
        //Second subpass: final composition with G-Buffer components.
        vk::AttachmentReference colorAttachmentRef = {0, vk::ImageLayout::eColorAttachmentOptimal};
        std::array<vk::AttachmentReference, 3> inputAttachmentRefs = {
            vk::AttachmentReference{1, vk::ImageLayout::eShaderReadOnlyOptimal},
            vk::AttachmentReference{2, vk::ImageLayout::eShaderReadOnlyOptimal},
            vk::AttachmentReference{3, vk::ImageLayout::eShaderReadOnlyOptimal},
        };

        subpassDescs[1].pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        subpassDescs[1].colorAttachmentCount = 1;
        subpassDescs[1].pColorAttachments = &colorAttachmentRef;
        subpassDescs[1].pDepthStencilAttachment = nullptr;
        subpassDescs[1].inputAttachmentCount = static_cast<uint32_t>(inputAttachmentRefs.size());
        subpassDescs[1].pInputAttachments = inputAttachmentRefs.data();
    }

    std::array<vk::SubpassDependency, 3> dependencies;

    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eTopOfPipe;
    dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependencies[0].srcAccessMask = vk::AccessFlagBits::eMemoryRead;
    dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
    dependencies[0].dependencyFlags = vk::DependencyFlagBits::eByRegion;

    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = 1;
    dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eFragmentShader;
    dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
    dependencies[1].dstAccessMask = vk::AccessFlagBits::eShaderRead;
    dependencies[1].dependencyFlags = vk::DependencyFlagBits::eByRegion;

    dependencies[2].srcSubpass = 1;
    dependencies[2].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[2].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependencies[2].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
    dependencies[2].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
    dependencies[2].dstAccessMask = vk::AccessFlagBits::eMemoryRead;
    dependencies[2].dependencyFlags = vk::DependencyFlagBits::eByRegion;

    

	vk::RenderPassCreateInfo createInfo = {
		vk::RenderPassCreateFlags(),
		static_cast<uint32_t>(attachmentDescs.size()),
		attachmentDescs.data(),
		static_cast<uint32_t>(subpassDescs.size()),
		subpassDescs.data(),
		static_cast<uint32_t>(dependencies.size()),
		dependencies.data()
	};
	auto pDevice = vg::pApp->getDevice();
	m_pRenderPass = fd::createRenderPass(pDevice, createInfo);
}

void MaterialDeferred::_createFramebuffer()
{
    std::array<vk::ImageView, 5> attachments = { 
        *(m_pAttachmentColor->getImageView()->getImageView()), 
        *(m_pAttachmentPos->getImageView()->getImageView()),
        *(m_pAttachmentNormal->getImageView()->getImageView()),
        *(m_pAttachmentAlbedo->getImageView()->getImageView()),
        *(m_pAttachmentDepthStencil->getImageView()->getImageView()),
        };

	vk::FramebufferCreateInfo createInfo = {
		vk::FramebufferCreateFlags(),                   //flags
		*m_pRenderPass,                                 //renderPass
		static_cast<uint32_t>(attachments.size()),      //attachmentCount
		attachments.data(),                             //pAttachments
		m_frameBufferWidth,                             //width
		m_frameBufferHeight,                            //height
		1u                                              //layers
	};
	auto pDevice = vg::pApp->getDevice();
	m_pFrameBuffer = fd::createFrameBuffer(pDevice, createInfo);
}

void MaterialDeferred::_createOtherPasses()
{
    m_pShaderDeferred = std::shared_ptr<vg::Shader>{new vg::Shader()};
    m_pShaderComposition = std::shared_ptr<vg::Shader>{new vg::Shader()};
	m_pPassDeferred = std::shared_ptr<vg::Pass>{ new vg::Pass(m_pShaderDeferred.get())};
	m_pPassComposition = std::shared_ptr<vg::Pass>{ new vg::Pass(m_pShaderComposition.get())};
	_addPass(m_pPassDeferred.get());
	_addPass(m_pPassComposition.get());
}

void MaterialDeferred::_initPasses()
{
    //deferred pass
    {
        auto pPass = m_pPassDeferred.get();
        pPass->setSubpass(0u);
	    vk::PipelineColorBlendAttachmentState attachmentStates[4] = {};
        for (uint32_t i = 0; i < 4; ++i)
        {
            attachmentStates[i].colorWriteMask = vk::ColorComponentFlagBits::eR | 
                vk::ColorComponentFlagBits::eG | 
                vk::ColorComponentFlagBits::eB | 
                vk::ColorComponentFlagBits::eA;
	        attachmentStates[0].blendEnable = VG_FALSE;
        }
	    
	    vk::PipelineColorBlendStateCreateInfo colorBlendState = {};
	    colorBlendState.attachmentCount = 4;
	    colorBlendState.pAttachments = attachmentStates;
	    pPass->setColorBlendInfo(colorBlendState);
        pPass->apply();
    }

    //composition pass
    {
        auto pPass = m_pPassComposition.get();
        pPass->setSubpass(1u);
        pPass->setPolygonMode(vg::PolygonMode::FILL);
	    pPass->setCullMode(vg::CullModeFlagBits::NONE);
	    pPass->setFrontFace(vg::FrontFaceType::COUNTER_CLOCKWISE);
	    vg::Pass::BuildInDataInfo buildInDataInfo;
	    buildInDataInfo.componentCount = 0u;
	    buildInDataInfo.pComponent = nullptr;
	    pPass->setBuildInDataInfo(buildInDataInfo);
        
	    vk::PipelineColorBlendAttachmentState attachmentStates[1] = {};
        for (uint32_t i = 0; i < 1; ++i)
        {
            attachmentStates[i].colorWriteMask = vk::ColorComponentFlagBits::eR | 
                vk::ColorComponentFlagBits::eG | 
                vk::ColorComponentFlagBits::eB | 
                vk::ColorComponentFlagBits::eA;
	        attachmentStates[i].blendEnable = VG_FALSE;
        }
	    
	    vk::PipelineColorBlendStateCreateInfo colorBlendState = {};
	    colorBlendState.attachmentCount = 1;
	    colorBlendState.pAttachments = attachmentStates;
	    pPass->setColorBlendInfo(colorBlendState);

	    vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
	    depthStencilState.depthTestEnable = VG_FALSE;
	    depthStencilState.depthWriteEnable = VG_FALSE;
	    depthStencilState.depthCompareOp = vk::CompareOp::eAlways;
	    pPass->setDepthStencilInfo(depthStencilState);

        pPass->setTexture("pos_input", m_pAttachmentPos.get(), 0u, vg::ShaderStageFlagBits::FRAGMENT, 
            vg::DescriptorType::INPUT_ATTACHMENT);
        pPass->setTexture("normal_input", m_pAttachmentNormal.get(), 1u, vg::ShaderStageFlagBits::FRAGMENT, 
            vg::DescriptorType::INPUT_ATTACHMENT);
        pPass->setTexture("albedo_input", m_pAttachmentAlbedo.get(), 2u, vg::ShaderStageFlagBits::FRAGMENT, 
            vg::DescriptorType::INPUT_ATTACHMENT);
        pPass->apply();
    }

    //main pass
    {
        auto pPass = m_pMainPass.get();
        pPass->setPolygonMode(vg::PolygonMode::FILL);
	    pPass->setCullMode(vg::CullModeFlagBits::NONE);
	    pPass->setFrontFace(vg::FrontFaceType::COUNTER_CLOCKWISE);
        vg::Pass::BuildInDataInfo buildInDataInfo;
	    buildInDataInfo.componentCount = 0u;
	    buildInDataInfo.pComponent = nullptr;
	    pPass->setBuildInDataInfo(buildInDataInfo);
        vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
	    depthStencilState.depthTestEnable = VG_TRUE;
	    depthStencilState.depthWriteEnable = VG_TRUE;
	    depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
        pPass->setTexture("color", m_pAttachmentColor.get(), 0u, vg::ShaderStageFlagBits::FRAGMENT);
        pPass->setTexture("depth", m_pAttachmentDepthStencil.get(), 1u, vg::ShaderStageFlagBits::FRAGMENT,
            vg::DescriptorType::COMBINED_IMAGE_SAMPLER, m_pAttachmentDepthStencil->getImageView("only_depth"));
        
        pPass->apply();
    }
}

vg::DimSepMesh2 *MaterialDeferred::_getRectMesh(vg::InstanceID id)
{
    auto &map = m_mapRectMeshes;
    const auto& iterator = map.find(id);
	if (iterator == map.cend())
	{
		std::shared_ptr<vg::DimSepMesh2> pNewMesh = std::shared_ptr<vg::DimSepMesh2>{
            new vg::DimSepMesh2(vg::MemoryPropertyFlagBits::HOST_VISIBLE)
        };
        map[id] = pNewMesh;
        return pNewMesh.get();
	}
	else
	{
		return iterator->second.get();
	}
}