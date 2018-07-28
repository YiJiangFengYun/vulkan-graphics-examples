#include "graphics/light/light_depth_target_cube.hpp"

namespace vg
{
//LightDistTargetCube
    const vk::Format LightDistTargetCube::DEFAULT_COLOR_FORMAT(vk::Format::eR32Sfloat);
    const vk::Format LightDistTargetCube::DEFAULT_DEPTH_FORMAT(vk::Format::eD32Sfloat);
    LightDistTargetCube::LightDistTargetCube(uint32_t framebufferWidth
        , uint32_t framebufferHeight
        , vk::Format colorImageFormat
        , vk::Format depthImageFormat
        )
        : OnceRenderTarget(framebufferWidth, framebufferHeight)
        , m_colorImageFormat(colorImageFormat)
        , m_depthImageFormat(depthImageFormat)
    {
        vk::ClearValue clearValueColor = {
            std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f}
        };
        vk::ClearValue clearValueDepthStencil = {
            vk::ClearDepthStencilValue(1.0f, 0)
        };
        std::array<vk::ClearValue, 2u> clearValues = { 
            clearValueColor,
            clearValueDepthStencil,
        };
        setClearValues(clearValues.data(), static_cast<uint32_t>(clearValues.size()));
        _createObjs();
    }

    vk::Format LightDistTargetCube::getColorImageFormat() const
    {
        return m_colorImageFormat;
    }
        
    vk::Format LightDistTargetCube::getDepthImageFormat() const
    {
        return m_depthImageFormat;
    }
    
    const TextureCubeColorAttachment *LightDistTargetCube::getColorTargetTexture() const
    {
        return m_pColorTargetTex.get();
    }

    const TextureDepthAttachment *LightDistTargetCube::getDepthTargetTexture() const
    {
        return m_pDepthTargetTex.get();
    }

    void LightDistTargetCube::_createObjs()
    {
        auto pDevice = pApp->getDevice();
        const auto framebufferWidth = m_framebufferWidth;
        const auto framebufferHeight = m_framebufferHeight;

        vk::AttachmentDescription colorAttachmentDes = {
            vk::AttachmentDescriptionFlags(),
            m_colorImageFormat,
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eShaderReadOnlyOptimal,
            vk::ImageLayout::eShaderReadOnlyOptimal,
        };

        //render pass.
        vk::AttachmentDescription depthAttachmentDes = {
            vk::AttachmentDescriptionFlags(),
            m_depthImageFormat,
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eDontCare,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eDepthStencilAttachmentOptimal,
        };

        std::array<vk::AttachmentDescription, 2> attachmentDess = { 
            colorAttachmentDes, 
            depthAttachmentDes,
        };

        const uint32_t subpassCount = 6u;
        vk::AttachmentReference colorAttachmentRef = {
            0u,
            vk::ImageLayout::eColorAttachmentOptimal,
        };
        vk::AttachmentReference depthAttachmentRef = {
            1u,
            vk::ImageLayout::eDepthStencilAttachmentOptimal
        };
        vk::SubpassDescription subpass = {
            vk::SubpassDescriptionFlags(),       //flags
            vk::PipelineBindPoint::eGraphics,    //pipelineBindPoint
            0u,                                   //inputAttachmentCount
            nullptr,                             //pInputAttachments
            1u,                                   //colorAttachmentCount
            &colorAttachmentRef,                 //pColorAttachments
            nullptr,                             //pResolveAttachments
            &depthAttachmentRef,                 //pDepthStencilAttachment
            0,                                   //preserveAttachmentCount
            nullptr                              //pPreserveAttachments
        };

        std::array<vk::SubpassDependency, 2u> dependencies = { 
            vk::SubpassDependency 
            {
                VK_SUBPASS_EXTERNAL,                                  
                0,                                                    
                vk::PipelineStageFlagBits::eTopOfPipe,                
                // vk::PipelineStageFlagBits::eEarlyFragmentTests | 
                    // vk::PipelineStageFlagBits::eLateFragmentTests | 
                    vk::PipelineStageFlagBits::eColorAttachmentOutput,    
                vk::AccessFlagBits::eShaderRead,                                    
                // vk::AccessFlagBits::eDepthStencilAttachmentRead | 
                //     vk::AccessFlagBits::eDepthStencilAttachmentWrite |
                    vk::AccessFlagBits::eColorAttachmentRead |
                    vk::AccessFlagBits::eColorAttachmentWrite,
                vk::DependencyFlagBits::eByRegion,
            },
            vk::SubpassDependency
            {
                0,                                                    
                VK_SUBPASS_EXTERNAL,                                  
                // vk::PipelineStageFlagBits::eEarlyFragmentTests | 
                    // vk::PipelineStageFlagBits::eLateFragmentTests | 
                    vk::PipelineStageFlagBits::eColorAttachmentOutput,
                vk::PipelineStageFlagBits::eBottomOfPipe,             
                // vk::AccessFlagBits::eDepthStencilAttachmentRead | 
                //     vk::AccessFlagBits::eDepthStencilAttachmentWrite | 
                    vk::AccessFlagBits::eColorAttachmentRead |
                    vk::AccessFlagBits::eColorAttachmentWrite,
                vk::AccessFlagBits::eShaderRead,                      
                vk::DependencyFlagBits::eByRegion         
            }
        };

        
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

        //Create textures
        auto pColorTex = new TextureCubeColorAttachment(m_colorImageFormat,
            framebufferWidth,
            framebufferHeight
            );

        m_pColorTargetTex = std::shared_ptr<TextureCubeColorAttachment>(pColorTex);

        auto pDepthTex = new TextureDepthAttachment(m_depthImageFormat,
            framebufferWidth,
            framebufferHeight
            );
        
        m_pDepthTargetTex = std::shared_ptr<TextureDepthAttachment>(pDepthTex);

        //frame buffer.
        std::array<vk::ImageView, 2u> attachments = {
            *(m_pColorTargetTex->getImageView()->getImageView()),
            *(m_pDepthTargetTex->getImageView()->getImageView()),
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