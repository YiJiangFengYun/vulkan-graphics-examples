#include "graphics/renderer/render_target_surface.hpp"

#include "graphics/app/app.hpp"
#include "graphics/texture/texture_depth_stencil_attachment.hpp"

namespace vg
{
    SurfaceRenderTarget::SurfaceRenderTarget(uint32_t swapchainImageViewCount
        , vk::ImageView *pSwapchainImageViews
        , vk::Format swapchainImageFormat
        , uint32_t swapchainImageWidth
        , uint32_t swapchainImageHeight)
        : RenderTarget(swapchainImageWidth
            , swapchainImageHeight
            , swapchainImageFormat
            , DEFAULT_DEPTH_STENCIL_FORMAT
            )
        , m_swapchainImageViewCount(swapchainImageViewCount)
        , m_pSwapchainImageViews(pSwapchainImageViews)
        , m_imageIndex(0u)
    {
        _createRenderPass();
        _createDepthStencilTex();
        _createFramebuffers();
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

    void SurfaceRenderTarget::setImageIndex(uint32_t imageIndex)
    {
        m_imageIndex = imageIndex;
        m_pFirstFramebuffer = m_pFirstFramebuffers[m_imageIndex].get();
        m_pSecondFramebuffer = m_pSecondFramebuffers[m_imageIndex].get();
    }

    void SurfaceRenderTarget::_createRenderPass()
    {
        vk::AttachmentDescription colorAttachment = {
            vk::AttachmentDescriptionFlags(),     //flags
            m_colorImageFormat,                   //format
            vk::SampleCountFlagBits::e1,          //samples
            vk::AttachmentLoadOp::eClear,         //loadOp
            vk::AttachmentStoreOp::eStore,        //storeOp
            vk::AttachmentLoadOp::eDontCare,      //stencilLoadOp
            vk::AttachmentStoreOp::eDontCare,     //stencilStoreOp
            vk::ImageLayout::eUndefined,          //initialLayout
            vk::ImageLayout::ePresentSrcKHR       //finalLayout
        };

        vk::AttachmentDescription depthAttachment = {
            vk::AttachmentDescriptionFlags(),
            m_depthStencilImageFormat,
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eDontCare,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eDepthStencilAttachmentOptimal
        };

        vk::AttachmentReference colorAttachmentRef = {
            uint32_t(0),
            vk::ImageLayout::eColorAttachmentOptimal
        };

        vk::AttachmentReference depthAttachmentRef = {
            uint32_t(1),
            vk::ImageLayout::eDepthStencilAttachmentOptimal
        };

        vk::SubpassDescription subpass = {
            vk::SubpassDescriptionFlags(),       //flags
            vk::PipelineBindPoint::eGraphics,    //pipelineBindPoint
            0,                                   //inputAttachmentCount
            nullptr,                             //pInputAttachments
            1,                                   //colorAttachmentCount
            &colorAttachmentRef,                 //pColorAttachments
            nullptr,                             //pResolveAttachments
            &depthAttachmentRef,                 //pDepthStencilAttachment
            0,                                   //preserveAttachmentCount
            nullptr                              //pPreserveAttachments
        };

        std::array<vk::SubpassDependency, 2> dependencies = { 
            vk::SubpassDependency 
            {
                VK_SUBPASS_EXTERNAL,                                  //srcSubpass
                0,                                                    //dstSubpass
                vk::PipelineStageFlagBits::eTopOfPipe,                //srcStageMask
                vk::PipelineStageFlagBits::eColorAttachmentOutput | 
                    vk::PipelineStageFlagBits::eEarlyFragmentTests |
                    vk::PipelineStageFlagBits::eLateFragmentTests,    //dstStageMask
                vk::AccessFlagBits::eMemoryRead,                                    //srcAccessMask
                vk::AccessFlagBits::eColorAttachmentRead |
                    vk::AccessFlagBits::eColorAttachmentWrite |
                    vk::AccessFlagBits::eDepthStencilAttachmentRead |
                    vk::AccessFlagBits::eDepthStencilAttachmentWrite,        //dstAccessMask
                vk::DependencyFlagBits::eByRegion                     //dependencyFlags
            },
            vk::SubpassDependency
            {
                0,                                                    //srcSubpass
                VK_SUBPASS_EXTERNAL,                                  //dstSubpass
                vk::PipelineStageFlagBits::eColorAttachmentOutput |
                    vk::PipelineStageFlagBits::eEarlyFragmentTests |
                    vk::PipelineStageFlagBits::eLateFragmentTests,    //srcStageMask
                vk::PipelineStageFlagBits::eBottomOfPipe,             //dstStageMask
                vk::AccessFlagBits::eColorAttachmentRead |
                    vk::AccessFlagBits::eColorAttachmentWrite |
                    vk::AccessFlagBits::eDepthStencilAttachmentRead |
                    vk::AccessFlagBits::eDepthStencilAttachmentWrite,        //srcAccessMask
                vk::AccessFlagBits::eMemoryRead,                      //dstAccessMask
                vk::DependencyFlagBits::eByRegion                     //dependencyFlags
            }
        };

        std::array<vk::AttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
        vk::RenderPassCreateInfo createInfo = {
            vk::RenderPassCreateFlags(),
            static_cast<uint32_t>(attachments.size()),
            attachments.data(),
            1u,
            &subpass,
            static_cast<uint32_t>(dependencies.size()),
            dependencies.data()
        };

        auto pDevice = pApp->getDevice();

        m_pFirstSurfaceRenderPass = fd::createRenderPass(pDevice, createInfo);
        m_pFirstRenderPass = m_pFirstSurfaceRenderPass.get();

        colorAttachment.loadOp = vk::AttachmentLoadOp::eLoad;
        colorAttachment.initialLayout = vk::ImageLayout::ePresentSrcKHR;
        attachments = { colorAttachment, depthAttachment };

        m_pSecondSurfaceRenderPass = fd::createRenderPass(pDevice, createInfo);
        m_pSecondRenderPass = m_pSecondSurfaceRenderPass.get(); 
    }

    void SurfaceRenderTarget::_createDepthStencilTex()
    {
        auto pTex = new TextureDepthStencilAttachment(
                m_depthStencilImageFormat,
                m_framebufferWidth,
                m_framebufferHeight
                );
        m_pDepthStencilAttachment = std::shared_ptr<BaseDepthStencilAttachment>(pTex);
    }

    void SurfaceRenderTarget::_createFramebuffers()
    {
        auto pDevice = pApp->getDevice();
        {
            m_pFirstFramebuffers.resize(m_swapchainImageViewCount);    
            for (uint32_t imageIndex = 0; imageIndex < m_swapchainImageViewCount; ++imageIndex)
            {
                std::array<vk::ImageView, 2> attachments;
                attachments = { *(m_pSwapchainImageViews + imageIndex), *m_pDepthStencilAttachment->getDepthStencilAttachmentImageView() };
        
                vk::FramebufferCreateInfo createInfo = {
                    vk::FramebufferCreateFlags(),                   //flags
                    *m_pFirstRenderPass,                            //renderPass
                    static_cast<uint32_t>(attachments.size()),      //attachmentCount
                    attachments.data(),                             //pAttachments
                    m_framebufferWidth,                             //width
                    m_framebufferHeight,                            //height
                    1u                                              //layers
                };
        
                m_pFirstFramebuffers[imageIndex] = fd::createFrameBuffer(pDevice, createInfo);
            }
    
            m_pFirstFramebuffer = m_pFirstFramebuffers[m_imageIndex].get();
        }

        {
            m_pSecondFramebuffers.resize(m_swapchainImageViewCount);    
            for (uint32_t imageIndex = 0; imageIndex < m_swapchainImageViewCount; ++imageIndex)
            {
                std::array<vk::ImageView, 2> attachments;
                attachments = { *(m_pSwapchainImageViews + imageIndex), *m_pDepthStencilAttachment->getDepthStencilAttachmentImageView() };
        
                vk::FramebufferCreateInfo createInfo = {
                    vk::FramebufferCreateFlags(),                   //flags
                    *m_pSecondRenderPass,                            //renderPass
                    static_cast<uint32_t>(attachments.size()),      //attachmentCount
                    attachments.data(),                             //pAttachments
                    m_framebufferWidth,                             //width
                    m_framebufferHeight,                            //height
                    1u                                              //layers
                };
        
                m_pSecondFramebuffers[imageIndex] = fd::createFrameBuffer(pDevice, createInfo);
            }
    
            m_pSecondFramebuffer = m_pSecondFramebuffers[m_imageIndex].get();
        }
        
        
       
    }
} //vg