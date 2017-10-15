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

	void BaseRenderer::_createPipelineForRender(std::shared_ptr<vk::PipelineLayout> &pPipelineLayout,
		std::shared_ptr<vk::Pipeline> &pPipeline,
		const RenderInfo& renderInfo,
		std::shared_ptr<BaseMesh> pMesh,
		std::shared_ptr<Material> pMaterial,
		uint32_t subMeshIndex,
		uint32_t passIndex)
	{
		//Create graphics pipeline create info. 
		vk::GraphicsPipelineCreateInfo createInfo = {};

		//Coustruct shader stage create info.
		auto pPass = pMaterial->getPassWithIndex(passIndex);
		auto pShader = pPass->_getShader();
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
		pMesh->_fillGraphicsPipelineCreateInfoForDraw(subMeshIndex, createInfo);

		//View port info.
		vk::Viewport viewport = {
			0.0f,                                     //x
			0.0f,                                     //y
			static_cast<float>(m_framebufferWidth),   //width
			static_cast<float>(m_framebufferHeight),  //height
			0.0f,                                     //minDepth
			1.0f                                      //maxDepth
		};

		vk::Rect2D scissor = {
			{                               //offset
				0,                             //x
				0                              //y
			}, 
			{                               //extent
				m_framebufferWidth,            //width
				m_framebufferHeight            //height
			} 
		};

		vk::PipelineViewportStateCreateInfo viewportStateCreateInfo = {
			vk::PipelineViewportStateCreateFlags(),                  //flags
			1u,                                                      //viewportCount
			&viewport,                                               //pViewports
			1u,                                                      //scissorCount
			&scissor                                                 //pScissors
		};
		createInfo.pViewportState = &viewportStateCreateInfo;

		//Rasterization info.
		vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
			vk::PipelineRasterizationStateCreateFlags(),  //flags
			VK_FALSE,                                     //depthClampEnable
			VK_FALSE,                                     //rasterizerDiscardEnable
			vk::PolygonMode::eFill,                       //polygonMode
			vk::CullModeFlagBits::eBack,                  //cullMode
			vk::FrontFace::eCounterClockwise,             //frontFace
			VK_FALSE,                                     //depthBiasEnable
			0.0f,                                         //depthBiasConstantFactor
			0.0f,                                         //depthBiasClamp
			0.0f,                                         //depthBiasSlopeFactor
			0.0f                                          //lineWidth
		};
		createInfo.pRasterizationState = &rasterizationStateCreateInfo;

		//Multisample info.
		vk::PipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {
			vk::PipelineMultisampleStateCreateFlags(),              //flags
			vk::SampleCountFlagBits::e1,                            //rasterizationSamples
			VK_FALSE,                                               //sampleShadingEnable
			0.0f,                                                   //minSampleShading
			nullptr,                                                //pSampleMask
			VK_FALSE,                                               //alphaToCoverageEnable
			VK_FALSE                                                //alphaToOneEnable
		};
		createInfo.pMultisampleState = &multisampleStateCreateInfo;

		//depth and stencil info.
		vk::PipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {
			vk::PipelineDepthStencilStateCreateFlags(),           //flags
			VK_TRUE,                                              //depthTestEnable
			VK_TRUE,                                              //depthWriteEnable
			vk::CompareOp::eLess,                                 //depthCompareOp
			VK_FALSE,                                             //depthBoundsTestEnable
			VK_FALSE,                                             //stencilTestEnable
			{},                                                   //front
			{},                                                   //back
			0.0f,                                                 //minDepthBounds
			0.0f                                                  //maxDepthBounds
		};
		createInfo.pDepthStencilState = &depthStencilStateCreateInfo;

		//color blend info
		vk::PipelineColorBlendAttachmentState colorBlendAttachmentState = {
			VK_FALSE,                                //blendEnable
			vk::BlendFactor::eZero,                  //srcColorBlendFactor
			vk::BlendFactor::eZero,                  //dstColorBlendFactor
			vk::BlendOp::eAdd,                       //colorBlendOp
			vk::BlendFactor::eZero,                  //srcAlphaBlendFactor
			vk::BlendFactor::eZero,                  //desAlphaBlendFactor
			vk::BlendOp::eAdd,                       //alphaBlendOp
			vk::ColorComponentFlagBits::eR
			| vk::ColorComponentFlagBits::eG
			| vk::ColorComponentFlagBits::eB
			| vk::ColorComponentFlagBits::eA  //colorWriteMask
		};
		
		vk::PipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {
			vk::PipelineColorBlendStateCreateFlags(),              //flags
			VK_FALSE,                                              //logicOpEnable
			vk::LogicOp::eClear,                                   //logicOp
			1u,                                                    //attachmentCount
			&colorBlendAttachmentState,                            //pAttachments
			{ 0.0f, 0.0f, 0.0f, 0.0f }                             //blendConstants
		};
		createInfo.pColorBlendState = &colorBlendStateCreateInfo;

		//pPass->_getDescriptorSetLayout();
		vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
			vk::PipelineLayoutCreateFlags(),             //flags
			1u,                                          //setLayoutCount
			pPass->_getDescriptorSetLayout().get(),      //pSetLayouts
			0,                                           //pushConstantRangeCount
			nullptr                                      //pPushConstantRanges
		};

		auto pDevice = pContext->getNativeDevice();
		pPipelineLayout = fd::createPipelineLayout(pDevice, pipelineLayoutCreateInfo);
		createInfo.layout = *pPipelineLayout;

		createInfo.pDynamicState = nullptr;
		createInfo.renderPass = *m_pRenderPass;
		createInfo.subpass = 0;

		pPipeline = fd::createGraphicsPipeline(pDevice, nullptr, createInfo);
	}

	void BaseRenderer::_recordCommandBufferForRender(const RenderInfo& renderInfo, 
		std::shared_ptr<vk::PipelineLayout> pPipelineLayout,
		std::shared_ptr<vk::Pipeline> pPipeline,
		std::shared_ptr<BaseMesh> pMesh,
		std::shared_ptr<Material> pMaterial,
		uint32_t subMeshIndex,
		uint32_t passIndex)
	{
		vk::CommandBufferBeginInfo beginInfo = {
			vk::CommandBufferUsageFlagBits::eSimultaneousUse,  //flags
			nullptr                                            //pInheritanceInfo
		};

		m_pCommandBuffer->begin(beginInfo);

		std::array<vk::ClearValue, 2> clearValues = { {
				vk::ClearValue(vk::ClearColorValue(std::array<float, 4>{{0.0f, 0.0f, 0.0f, 0.0f}})),
				vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0u))
			} };
		vk::RenderPassBeginInfo renderPassBeginInfo = {
			*m_pRenderPass,                                   //renderPass
			*m_pFrameBuffer,                                  //framebuffer
			vk::Rect2D(                                     //renderArea
				vk::Offset2D(0, 0),
				vk::Extent2D(0u, 0u)
			),
			static_cast<uint32_t>(clearValues.size()),      //clearValueCount
			clearValues.data()                              //pClearValues
		};

		m_pCommandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
		m_pCommandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, *pPipeline);

		auto pPass = pMaterial->getPassWithIndex(passIndex);
		m_pCommandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, *pPipelineLayout, 0u, *pPass->_getDescriptorSet(), nullptr);

		pMesh->_fillCommandBufferForDraw(subMeshIndex, *m_pCommandBuffer);

		m_pCommandBuffer->drawIndexed(pMesh->getIndexCount(subMeshIndex), 1u, 0u, 0u, 0u);

		m_pCommandBuffer->endRenderPass();

		m_pCommandBuffer->end();
	}

	void BaseRenderer::_init()
	{
		_createRenderPass();
		_createFramebuffer();
		_createCommandBuffer();
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

	void BaseRenderer::_createCommandBuffer()
	{
		auto pCommandPool = pContext->getCommandPool();
		vk::CommandBufferAllocateInfo allocateInfo = {
			*pCommandPool,                //commandPool
			vk::CommandBufferLevel::ePrimary,          //level
			1u                                         //commandBufferCount
		};

		auto pDevice = pContext->getNativeDevice();
		m_pCommandBuffer = fd::allocateCommandBuffer(pDevice, pCommandPool, allocateInfo);
	}
}