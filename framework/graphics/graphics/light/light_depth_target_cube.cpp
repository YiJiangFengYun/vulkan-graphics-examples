#include "graphics/light/light_depth_target_cube.hpp"

namespace vg
{
//LightDepthCubeFaceTarget
    LightDepthCubeFaceTarget::LightDepthCubeFaceTarget(uint32_t framebufferWidth
        , uint32_t framebufferHeight
        , vk::Format depthImageFormat
        , const vk::ImageView *pFaceImageView
        )
        : PreZTarget(framebufferWidth, framebufferHeight, depthImageFormat)
        , m_pFaceImageView(pFaceImageView)
    {
        _createObjs();
    }

    void LightDepthCubeFaceTarget::_createObjs()
    {
        auto pDevice = pApp->getDevice();
        const auto framebufferWidth = m_framebufferWidth;
        const auto framebufferHeight = m_framebufferHeight;

        //render pass.
        vk::AttachmentDescription depthAttachmentDes = {
            vk::AttachmentDescriptionFlags(),
            m_depthImageFormat,
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eDepthStencilReadOnlyOptimal,
        };

        vk::AttachmentReference depthAttachmentRef = {
            uint32_t(0),
            vk::ImageLayout::eDepthStencilAttachmentOptimal
        };

        vk::SubpassDescription subpass = {
            vk::SubpassDescriptionFlags(),       //flags
            vk::PipelineBindPoint::eGraphics,    //pipelineBindPoint
            0,                                   //inputAttachmentCount
            nullptr,                             //pInputAttachments
            0,                                   //colorAttachmentCount
            nullptr,                             //pColorAttachments
            nullptr,                             //pResolveAttachments
            &depthAttachmentRef,                 //pDepthStencilAttachment
            0,                                   //preserveAttachmentCount
            nullptr                              //pPreserveAttachments
        };

        std::array<vk::SubpassDependency, 2> dependencies = { 
            vk::SubpassDependency 
            {
                VK_SUBPASS_EXTERNAL,                                  
                0,                                                    
                vk::PipelineStageFlagBits::eTopOfPipe,                
                vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests,    
                vk::AccessFlagBits::eShaderRead,                                    
                vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite,       
                vk::DependencyFlagBits::eByRegion                     
            },
            vk::SubpassDependency
            {
                0,                                                    
                VK_SUBPASS_EXTERNAL,                                  
                vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests,    
                vk::PipelineStageFlagBits::eBottomOfPipe,             
                vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite,
                vk::AccessFlagBits::eShaderRead,                      
                vk::DependencyFlagBits::eByRegion         
            }
        };

        std::array<vk::AttachmentDescription, 1> attachmentDess = { depthAttachmentDes };
        vk::RenderPassCreateInfo renderPassCreateInfo = {
            vk::RenderPassCreateFlags(),
            static_cast<uint32_t>(attachmentDess.size()),
            attachmentDess.data(),
            1u,
            &subpass,
            static_cast<uint32_t>(dependencies.size()),
            dependencies.data()
        };

        m_pMyRenderPass = fd::createRenderPass(pDevice, renderPassCreateInfo);
        m_pRenderPass = m_pMyRenderPass.get();

        //frame buffer.
        std::array<vk::ImageView, 1> attachments = {
             *m_pFaceImageView,
        };

        vk::FramebufferCreateInfo frameBufferCreateInfo = {
            vk::FramebufferCreateFlags(),      
            *m_pRenderPass,                                
            static_cast<uint32_t>(attachments.size()),      
            attachments.data(),                             
            framebufferWidth,                             
            framebufferHeight,                            
            1u,                                  
        };

        m_pMyFramebuffer = fd::createFrameBuffer(pDevice, frameBufferCreateInfo);
        m_pFramebuffer = m_pMyFramebuffer.get();
    }

//LightDepthCubeTargets
    const vk::Format LightDepthCubeTargets::DEFAULT_DEPTH_FORMAT(vk::Format::eD32Sfloat);
    LightDepthCubeTargets::LightDepthCubeTargets(uint32_t framebufferWidth
        , uint32_t framebufferHeight
        , vk::Format depthImageFormat
        )
        : m_framebufferWidth(framebufferWidth)
        , m_framebufferHeight(framebufferHeight)
        , m_depthImageFormat(depthImageFormat)
        , m_pDepthTargetTex()
        , m_pFaceTargets()
    {
        _createObjs();
    }

    vk::Format LightDepthCubeTargets::getDepthImageFormat() const
    {
        return m_depthImageFormat;
    }

    const TextureCubeDepthAttachment *LightDepthCubeTargets::getDepthTargetTexture() const
    {
        return m_pDepthTargetTex.get();
    }

    const typename LightDepthCubeTargets::PFaceTargetArray &LightDepthCubeTargets::getFaceTargets() const
    {
        return m_pFaceTargets;
    }

    void LightDepthCubeTargets::_createObjs()
    {
        //Create texture.
        auto pDevice = pApp->getDevice();
        const auto framebufferWidth = m_framebufferWidth;
        const auto framebufferHeight = m_framebufferHeight;
        //depth attachment
        auto pTex = new TextureCubeDepthAttachment(m_depthImageFormat,
            m_framebufferWidth,
            m_framebufferHeight
            );

        m_pDepthTargetTex = std::shared_ptr<TextureCubeDepthAttachment>(pTex);

        //Create face image views.
        uint32_t faceCount = static_cast<uint32_t>(CubemapFace::RANGE_SIZE);
        std::array<const Texture::ImageView *, static_cast<size_t>(CubemapFace::RANGE_SIZE)> faceImageViews;
        for (uint32_t i = 0; i < faceCount; ++i) {
            vk::ImageSubresourceRange subResRange = {
                vk::ImageAspectFlagBits::eDepth,          //aspectMask
                0u,                                       //baseMipLevel
                1u,                                       //levelCount
                i,                                       //baseArrayLayer
                1u,                                       //layerCount
            };
            Texture::ImageViewCreateInfo createInfo = {
                vk::ComponentMapping(),
                subResRange,
            };
            faceImageViews[i] = m_pDepthTargetTex->createImageView("face_" + std::to_string(i), createInfo);
        }

        //Create face render targets.
        for (uint32_t i = 0; i < faceCount; ++i) {
            m_pFaceTargets[i] = std::shared_ptr<LightDepthCubeFaceTarget>{ new LightDepthCubeFaceTarget{
                m_framebufferWidth,
                m_framebufferHeight,
                m_depthImageFormat,
                faceImageViews[i]->getImageView(),
            }};
        }
    }
} //vg