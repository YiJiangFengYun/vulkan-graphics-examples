#include "graphics/renderer/renderer_color_texture.hpp"

namespace vg
{
    ColorTexRenderer::ColorTexRenderer(BaseColorAttachment *pColorAttachmentTex)
        : Renderer()
        , m_pColorAttchment(pColorAttachmentTex)
    {
        m_framebufferWidth = pColorAttachmentTex->getColorAttachmentWidth();
		m_framebufferHeight = pColorAttachmentTex->getColorAttachmentHeight();
        m_colorImageFormat = pColorAttachmentTex->getColorAttachmentFormat();
		m_depthStencilImageFormat = DEFAULT_DEPTH_STENCIL_FORMAT;
        _createRenderPass();
		_createDepthStencilTex();
		_createFramebuffer();
    }

    void ColorTexRenderer::_createRenderPass()
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
			    vk::AccessFlagBits::eMemoryRead,                                    //srcAccessMask
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
		m_pRenderPass = fd::createRenderPass(pDevice, createInfo);

        m_pCurrRenderPass = m_pRenderPass.get();
    }

	void ColorTexRenderer::_createDepthStencilTex()
    {
        auto pTex = new TextureDepthStencilAttachment(
			    m_depthStencilImageFormat,
				m_framebufferWidth,
				m_framebufferHeight
			    );
		m_pDepthStencilAttachment = std::shared_ptr<BaseDepthStencilAttachment>(pTex);
    }
		
    void ColorTexRenderer::_createFramebuffer()
    {
		auto pDevice = pApp->getDevice();
        std::array<vk::ImageView, 2> attachments;
		attachments = { *m_pColorAttchment->getColorAttachmentImageView(), *m_pDepthStencilAttachment->getDepthStencilAttachmentImageView() };

		vk::FramebufferCreateInfo createInfo = {
			vk::FramebufferCreateFlags(),                   //flags
			*m_pRenderPass,                                 //renderPass
			static_cast<uint32_t>(attachments.size()),      //attachmentCount
			attachments.data(),                             //pAttachments
			m_framebufferWidth,                             //width
			m_framebufferHeight,                            //height
			1u                                              //layers
		};

		m_pFrameBuffer = fd::createFrameBuffer(pDevice, createInfo);
        m_pCurrFrameBuffer = m_pFrameBuffer.get();
    }


} //vg