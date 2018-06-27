#include "graphics/renderer/renderer_target_color_texture.hpp"

#include "graphics/app/app.hpp"
#include "graphics/texture/texture_depth_stencil_attachment.hpp"

namespace vg
{
    ColorTexRendererTarget::ColorTexRendererTarget(BaseColorAttachment *pColorAttachmentTex)
        : RendererTarget(pColorAttachmentTex->getColorAttachmentWidth()
            , pColorAttachmentTex->getColorAttachmentHeight()
            , pColorAttachmentTex->getColorAttachmentFormat()
            , DEFAULT_DEPTH_STENCIL_FORMAT
            )
        , m_pColorAttchment(pColorAttachmentTex)
    {
        _createRenderPass();
        _createDepthStencilTex();
        _createFramebuffer();
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

    void ColorTexRendererTarget::_createRenderPass()
    {
        vk::AttachmentDescription colorAttachment = {
            vk::AttachmentDescriptionFlags(),     //flags
            m_colorImageFormat,                   //format
            vk::SampleCountFlagBits::e1,          //samples
            vk::AttachmentLoadOp::eClear,         //loadOp
            vk::AttachmentStoreOp::eStore,        //storeOp
            vk::AttachmentLoadOp::eDontCare,      //stencilLoadOp
            vk::AttachmentStoreOp::eDontCare,     //stencilStoreOp
            m_pColorAttchment->getColorAttachmentLayout(),    //initialLayout
            m_pColorAttchment->getColorAttachmentLayout()     //finalLayout
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
                vk::PipelineStageFlagBits::eColorAttachmentOutput,    //dstStageMask
                vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eMemoryRead,    //srcAccessMask
                vk::AccessFlagBits::eColorAttachmentRead |
                    vk::AccessFlagBits::eColorAttachmentWrite,        //dstAccessMask
                vk::DependencyFlagBits::eByRegion                     //dependencyFlags
            },
            vk::SubpassDependency
            {
                0,                                                    //srcSubpass
                VK_SUBPASS_EXTERNAL,                                  //dstSubpass
                vk::PipelineStageFlagBits::eColorAttachmentOutput,    //srcStageMask
                vk::PipelineStageFlagBits::eBottomOfPipe,             //dstStageMask
                vk::AccessFlagBits::eColorAttachmentRead |
                    vk::AccessFlagBits::eColorAttachmentWrite,        //srcAccessMask
                vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eMemoryRead, //dstAccessMask
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
        m_pMyFirstRenderPass = fd::createRenderPass(pDevice, createInfo);
        m_pFirstRenderPass = m_pMyFirstRenderPass.get();

        colorAttachment.loadOp = vk::AttachmentLoadOp::eLoad;
        attachments = { colorAttachment, depthAttachment };
        m_pMySecondRenderPass = fd::createRenderPass(pDevice, createInfo);
        m_pSecondRenderPass = m_pMySecondRenderPass.get();
    }

    void ColorTexRendererTarget::_createDepthStencilTex()
    {
        auto pTex = new TextureDepthStencilAttachment(
                m_depthStencilImageFormat,
                m_framebufferWidth,
                m_framebufferHeight
                );
        m_pMyDepthStencilAttachment = std::shared_ptr<TextureDepthStencilAttachment>(pTex);
        m_pDepthStencilAttachment = m_pMyDepthStencilAttachment->getImageView()->getImageView();
    }
        
    void ColorTexRendererTarget::_createFramebuffer()
    {
        auto pDevice = pApp->getDevice();
        {
            std::array<vk::ImageView, 2> attachments;
            attachments = { *m_pColorAttchment->getColorAttachmentImageView(), *m_pDepthStencilAttachment };
    
            vk::FramebufferCreateInfo createInfo = {
                vk::FramebufferCreateFlags(),                   //flags
                *m_pFirstRenderPass,                            //renderPass
                static_cast<uint32_t>(attachments.size()),      //attachmentCount
                attachments.data(),                             //pAttachments
                m_framebufferWidth,                             //width
                m_framebufferHeight,                            //height
                1u                                              //layers
            };

            m_pMyFirstFramebuffer = fd::createFrameBuffer(pDevice, createInfo);
            m_pFirstFramebuffer = m_pMyFirstFramebuffer.get();
        }

        {
            std::array<vk::ImageView, 2> attachments;
            attachments = { *m_pColorAttchment->getColorAttachmentImageView(), *m_pDepthStencilAttachment };
    
            vk::FramebufferCreateInfo createInfo = {
                vk::FramebufferCreateFlags(),                   //flags
                *m_pSecondRenderPass,                                 //renderPass
                static_cast<uint32_t>(attachments.size()),      //attachmentCount
                attachments.data(),                             //pAttachments
                m_framebufferWidth,                             //width
                m_framebufferHeight,                            //height
                1u                                              //layers
            };

            m_pMySecondFramebuffer = fd::createFrameBuffer(pDevice, createInfo);
            m_pSecondFramebuffer = m_pMySecondFramebuffer.get();
        }
        
    }


} //vg