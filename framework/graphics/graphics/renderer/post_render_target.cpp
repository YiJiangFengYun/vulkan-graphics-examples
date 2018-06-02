#include "graphics/renderer/post_render_target.hpp"

namespace vg
{
    const vk::Format PostRenderTarget::DEFAULT_COLOR_FORMAT(vk::Format::eR8G8B8A8Unorm);
    const vk::Format PostRenderTarget::DEFAULT_DEPTH_STENCIL_FORMAT(vk::Format::eD32SfloatS8Uint);

    PostRenderTarget::PostRenderTarget(uint32_t framebufferWidth
        , uint32_t framebufferHeight
        , vk::Format colorImageFormat
        , vk::Format depthStencilImageFormat
        )
        : BaseRenderTarget(framebufferWidth, framebufferHeight)
        , m_colorImageFormat(colorImageFormat)
        , m_depthStencilImageFormat(depthStencilImageFormat)
        , m_pColorAttachment()
        , m_pDepthStencilAttachment()
        , m_pRenderPass()
        , m_pFramebuffer()
    {
        _createObjs();

        vk::ClearValue clearValueColor = {
            std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f}
        };
        vk::ClearValue clearValueDepthStencil = {
            vk::ClearDepthStencilValue(1.0f, 0)
        };
        std::array<vk::ClearValue, 2> clearValues = { clearValueColor
            , clearValueDepthStencil
        };
        setClearValues(clearValues.data(), static_cast<uint32_t>(clearValues.size()));
    }
        
    vk::Format PostRenderTarget::getColorImageFormat() const
    {
        return m_colorImageFormat;
    }

    vk::Format PostRenderTarget::getDepthStencilImageFormat() const
    {
        return m_depthStencilImageFormat;
    }
        
    const Texture2DColorAttachment *PostRenderTarget::getColorAttachment() const
    {
        return m_pColorAttachment.get();
    }

    const TextureDepthStencilAttachment *PostRenderTarget::getDepthStencilAttachment() const
    {
        return m_pDepthStencilAttachment.get();
    }
        
    const vk::RenderPass *PostRenderTarget::getRenderPass() const
    {
        return m_pRenderPass.get();
    }

    const vk::Framebuffer *PostRenderTarget::getFramebuffer() const
    {
        return m_pFramebuffer.get();
    }

    void PostRenderTarget::_createObjs()
    {
        auto pDevice = pApp->getDevice();
        const auto framebufferWidth = m_framebufferWidth;
        const auto framebufferHeight = m_framebufferHeight;
        //color attachment
        auto pColorTex = new Texture2DColorAttachment(
            m_colorImageFormat,
            framebufferWidth,
            framebufferHeight
            );
        m_pColorAttachment = std::shared_ptr<Texture2DColorAttachment>(pColorTex);

        auto pDepthStencilTex = new TextureDepthStencilAttachment(
            m_depthStencilImageFormat,
            framebufferWidth,
            framebufferHeight
            );
        m_pDepthStencilAttachment = std::shared_ptr<TextureDepthStencilAttachment>(pDepthStencilTex);

        //render pass.
        auto pImage = m_pColorAttachment->getImage();
        vk::AttachmentDescription colorAttachmentDes = {
            vk::AttachmentDescriptionFlags(),
            m_colorImageFormat,
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            pImage->getInfo().layout,
        };

        pImage = m_pDepthStencilAttachment->getImage();
        vk::AttachmentDescription depthStencilAttachmentDes = {
            vk::AttachmentDescriptionFlags(),
            m_depthStencilImageFormat,
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eDontCare,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            pImage->getInfo().layout,
        };

        vk::AttachmentReference colorAttachmentRef = {
            0u,
            vk::ImageLayout::eColorAttachmentOptimal,
        };

        vk::AttachmentReference depthStencilAttachmentRef = {
            1u,
            vk::ImageLayout::eDepthStencilAttachmentOptimal,
        };

        vk::SubpassDescription subpass = {
            vk::SubpassDescriptionFlags(),       //flags
            vk::PipelineBindPoint::eGraphics,    //pipelineBindPoint
            0,                                   //inputAttachmentCount
            nullptr,                             //pInputAttachments
            1,                                   //colorAttachmentCount
            &colorAttachmentRef,                 //pColorAttachments
            nullptr,                             //pResolveAttachments
            &depthStencilAttachmentRef,          //pDepthStencilAttachment
            0,                                   //preserveAttachmentCount
            nullptr                              //pPreserveAttachments
        };

        std::array<vk::SubpassDependency, 2> dependencies = { 
            vk::SubpassDependency 
            {
                VK_SUBPASS_EXTERNAL,                                  
                0,                                                    
                vk::PipelineStageFlagBits::eTopOfPipe,                
                vk::PipelineStageFlagBits::eColorAttachmentOutput, 
                vk::AccessFlagBits::eShaderRead,                                    
                vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,       
                vk::DependencyFlagBits::eByRegion                     
            },
            vk::SubpassDependency
            {
                0,                                                    
                VK_SUBPASS_EXTERNAL,                                  
                vk::PipelineStageFlagBits::eColorAttachmentOutput,    
                vk::PipelineStageFlagBits::eBottomOfPipe,             
                vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
                vk::AccessFlagBits::eShaderRead,                      
                vk::DependencyFlagBits::eByRegion         
            }
        };

        std::array<vk::AttachmentDescription, 2> attachmentDess = { colorAttachmentDes, depthStencilAttachmentDes };
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
        std::array<vk::ImageView, 2> attachments = {
             *(m_pColorAttachment->getColorAttachmentImageView()),
             *(m_pDepthStencilAttachment->getDepthStencilAttachmentImageView()),
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