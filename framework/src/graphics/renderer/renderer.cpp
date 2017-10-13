#include "graphics/renderer/renderer.hpp"

namespace kgs
{
	const vk::Format BaseRenderer::DEFAULT_DEPTH_STENCIL_FORMAT(vk::Format::eD32SfloatS8Uint);

	BaseRenderer::BaseRenderer(std::shared_ptr<vk::ImageView> pSwapchainImageView
		, vk::Format swapchainImageFormat
		, uint32_t swapchainImageWidth
		, uint32_t swapchainImageHeight
	)
		: m_pSwapchainImageView(pSwapchainImageView)
		, m_swapchainImageFormat(swapchainImageFormat)
		, m_colorImageFormat(swapchainImageFormat)
		, m_depthStencilImageFormat(DEFAULT_DEPTH_STENCIL_FORMAT)
		, m_framebufferWidth(swapchainImageWidth)
		, m_framebufferHeight(swapchainImageHeight)
	{
		_init();
	}

	BaseRenderer::BaseRenderer(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
	)
		: m_pColorTexture(pColorAttachmentTex)
		, m_colorImageFormat(pColorAttachmentTex->_getVKFormat())
		, m_depthStencilImageFormat(DEFAULT_DEPTH_STENCIL_FORMAT)
		, m_framebufferWidth(pColorAttachmentTex->getWidth())
		, m_framebufferHeight(pColorAttachmentTex->getHeight())
	{
		_init();
	}

	void BaseRenderer::render(RenderInfo renderInfo)
	{
		_render(renderInfo);
	}

	void BaseRenderer::_render(RenderInfo renderInfo)
	{
		
	}

	void BaseRenderer::_render_createGraphicsPipeline(std::shared_ptr<BaseVisualObject> pVisualObject, uint32_t passIndex)
	{
		//Create graphics pipeline create info. 
		vk::GraphicsPipelineCreateInfo createInfo = {};

		//Coustruct shader stage create info.
		auto pShader = pVisualObject->getMaterial()->getPassWithIndex(passIndex)->_getShader();
		vk::PipelineShaderStageCreateInfo vertShaderStageInfo = {
			vk::PipelineShaderStageCreateFlags(),
			vk::ShaderStageFlagBits::eVertex,
			*pShader->getVertShaderModule(),
			"main"
		};

		vk::PipelineShaderStageCreateInfo fragShaderStageInfo = {
			vk::PipelineShaderStageCreateFlags(),
			vk::ShaderStageFlagBits::eFragment,
			*pShader->getFragShaderModule(),
			"main"
		};

		vk::PipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		createInfo.stageCount = 2u;
		createInfo.pStages = shaderStages;

		//Fill binding description and attributeDescriptions for one sub mesh of the mesh.
		auto subMeshIndex = pVisualObject->getSubMeshIndex();
		pVisualObject->getMesh()->_fillGraphicsPipelineCreateInfoForDraw(subMeshIndex, createInfo);

		//View port info.

		

	}

	void BaseRenderer::_init()
	{
		_createRenderPass();
		_createFramebuffer();
	}

	void BaseRenderer::_createRenderPass()
	{
		vk::AttachmentDescription colorAttachment = {
			vk::AttachmentDescriptionFlags(),
			m_colorImageFormat,
			vk::SampleCountFlagBits::e1,
			vk::AttachmentLoadOp::eClear,
			vk::AttachmentStoreOp::eStore,
			vk::AttachmentLoadOp::eDontCare,
			vk::AttachmentStoreOp::eDontCare,
			vk::ImageLayout::eUndefined,
			m_pColorTexture == nullptr ? vk::ImageLayout::ePresentSrcKHR : vk::ImageLayout::eShaderReadOnlyOptimal
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
			vk::SubpassDescriptionFlags(),
			vk::PipelineBindPoint::eGraphics,
			0,
			nullptr,
			1,
			&colorAttachmentRef,
			nullptr,
			&depthAttachmentRef,
			0,
			nullptr
		};

		vk::SubpassDependency dependency = {
			VK_SUBPASS_EXTERNAL,
			0,
			vk::PipelineStageFlagBits::eColorAttachmentOutput,
			vk::PipelineStageFlagBits::eColorAttachmentOutput,
			vk::AccessFlags(),
			vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
			vk::DependencyFlags()
		};

		std::array<vk::AttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
		vk::RenderPassCreateInfo createInfo = {
			vk::RenderPassCreateFlags(),
			static_cast<uint32_t>(attachments.size()),
			attachments.data(),
			static_cast<uint32_t>(1),
			&subpass,
			static_cast<uint32_t>(1),
			&dependency
		};

		auto pDevice = pContext->getNativeDevice();
		m_pRenderPass = fd::createRenderPass(pDevice, createInfo);
	}

	void BaseRenderer::_createFramebuffer()
	{
		std::array<vk::ImageView, 2> attachments;
		if (m_pColorTexture != nullptr)
		{
			attachments = { *m_pColorTexture->_getImageView(), *m_pDepthStencilTexture->_getImageView() };
		}
		else
		{
			attachments = { *m_pSwapchainImageView, *m_pDepthStencilTexture->_getImageView() };
		}

		vk::FramebufferCreateInfo createInfo = {
			vk::FramebufferCreateFlags(),
			*m_pRenderPass,
			static_cast<uint32_t>(attachments.size()),
			attachments.data(),
			m_framebufferWidth,
			m_framebufferHeight,
			1u
		};

		auto pDevice = pContext->getNativeDevice();
		m_pFrameBuffer = fd::createFrameBuffer(pDevice, createInfo);
	}
}