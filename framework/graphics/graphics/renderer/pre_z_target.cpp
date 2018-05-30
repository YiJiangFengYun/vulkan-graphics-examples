#include "graphics/renderer/pre_z_target.hpp"

namespace vg
{
    const vk::Format PreZTarget::DEFAULT_DEPTH_FORMAT(vk::Format::eD32Sfloat);

    PreZTarget::PreZTarget(uint32_t framebufferWidth
        , uint32_t framebufferHeight
        , vk::Format depthImageFormat
        )
        : m_framebufferWidth(framebufferWidth)
        , m_framebufferHeight(framebufferHeight)
        , m_depthImageFormat(depthImageFormat)
        , m_renderArea()
        , m_pDepthAttachment()
        , m_pRenderPass()
        , m_pFramebuffer()
    {
        _createObjs();
    }

    uint32_t PreZTarget::getFramebufferWidth() const
    {
        return m_framebufferWidth;
    }

    uint32_t PreZTarget::getFramebufferHeight() const
    {
        return m_framebufferHeight;
    }

    vk::Format PreZTarget::getDepthImageFormat() const
    {
        return m_depthImageFormat;
    }

    fd::Rect2D PreZTarget::getRenderArea() const
    {
        return m_renderArea;
    }

    const Texture2DDepthAttachment *PreZTarget::getDepthAttachment() const
    {
        return m_pDepthAttachment.get();
    }

    const vk::RenderPass *PreZTarget::getRenderPass() const
    {
        return m_pRenderPass.get();
    }
        
    const vk::Framebuffer *PreZTarget::getFramebuffer() const
    {
        return m_pFramebuffer.get();
    }

    void PreZTarget::_createObjs()
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
        m_pDepthAttachment = std::shared_ptr<Texture2DDepthAttachment>(pTex);

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
            m_pDepthAttachment->getDepthStencilAttachmentLayout(),
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

        m_pRenderPass = fd::createRenderPass(pDevice, renderPassCreateInfo);

        //frame buffer.
        std::array<vk::ImageView, 1> attachments = {
             *(m_pDepthAttachment->getDepthStencilAttachmentImageView()),
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

        m_pFramebuffer = fd::createFrameBuffer(pDevice, frameBufferCreateInfo);
    }
} //vg