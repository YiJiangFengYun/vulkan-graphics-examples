#include "graphics/renderer/renderer_pre_depth_target.hpp"

namespace vg
{
    RendererPreDepthTarget::RendererPreDepthTarget(uint32_t framebufferWidth
        , uint32_t framebufferHeight
        , vk::Format depthImageFormat
        )
        : PreDepthTarget(framebufferWidth, framebufferHeight, depthImageFormat)
    {
        _createObjs();
    }

    const Texture2DDepthAttachment *RendererPreDepthTarget::getDepthTargetTexture() const
    {
        return m_pDepthTargetTex.get();
    }

    void RendererPreDepthTarget::_createObjs()
    {
        auto pDevice = pApp->getDevice();
        const auto framebufferWidth = m_framebufferWidth;
        const auto framebufferHeight = m_framebufferHeight;
        //depth attachment
        auto pTex = new Texture2DDepthAttachment(
                m_depthImageFormat,
                framebufferWidth,
                framebufferHeight
                );

        m_pDepthTargetTex = std::shared_ptr<Texture2DDepthAttachment>(pTex);
        m_pDepthAttachment = m_pDepthTargetTex->getImageView()->getImageView();

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
            vk::ImageLayout::eDepthStencilReadOnlyOptimal
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
            nullptr,                 //pColorAttachments
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
             *m_pDepthAttachment,
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
} //vg