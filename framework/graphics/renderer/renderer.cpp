#include "graphics/renderer/renderer.hpp"

#include "graphics/vertex_data/util.hpp"

namespace vg
{
	const vk::Format BaseRenderer::DEFAULT_DEPTH_STENCIL_FORMAT(vk::Format::eD32SfloatS8Uint);

	BaseRenderer::BaseRenderer(std::shared_ptr<vk::ImageView> pSwapchainImageView
		, vk::Format swapchainImageFormat
		, uint32_t swapchainImageWidth
		, uint32_t swapchainImageHeight
	)
		: Base(BaseType::RENDERER)
		, m_pSwapchainImageView(pSwapchainImageView)
		, m_swapchainImageFormat(swapchainImageFormat)
		, m_colorImageFormat(swapchainImageFormat)
		, m_depthStencilImageFormat(DEFAULT_DEPTH_STENCIL_FORMAT)
		, m_framebufferWidth(swapchainImageWidth)
		, m_framebufferHeight(swapchainImageHeight)
		, m_clearValueColor(0.0f)
		, m_clearValueDepth(1.0f)
		, m_clearValueStencil(0u)
		, m_renderArea(0.0f, 0.0f, 1.0f, 1.0f)
	{
		_init();
	}

	BaseRenderer::BaseRenderer(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
	)
		: Base(BaseType::RENDERER)
		, m_pColorTexture(pColorAttachmentTex)
		, m_colorImageFormat(pColorAttachmentTex->_getVKFormat())
		, m_depthStencilImageFormat(DEFAULT_DEPTH_STENCIL_FORMAT)
		, m_framebufferWidth(pColorAttachmentTex->getWidth())
		, m_framebufferHeight(pColorAttachmentTex->getHeight())
		, m_clearValueColor(0.0f)
		, m_clearValueDepth(1.0f)
		, m_clearValueStencil(0u)
		, m_renderArea(0.0f, 0.0f, 1.0f, 1.0f)
	{
		_init();
	}

	BaseRenderer::~BaseRenderer()
	{
		//_freeGraphicsQueue();
	}

	Bool32 BaseRenderer::isValidForRender()
	{
		return _isValidForRender();
	}

	void BaseRenderer::render(const RenderInfo &info, RenderResultInfo &resultInfo)
	{
		_render(info, resultInfo);
	}

	uint32_t BaseRenderer::getFramebufferWidth()
	{
		return m_framebufferWidth;
	}

	uint32_t BaseRenderer::getFramebufferHeight()
	{
		return m_framebufferHeight;
	}

	vk::Format BaseRenderer::getColorImageFormat()
	{
		return m_colorImageFormat;
	}

	vk::Format BaseRenderer::getDepthStencilImageFormat()
	{
		return m_depthStencilImageFormat;
	}

	const Color &BaseRenderer::getClearValueColor() const
	{
		return m_clearValueColor;
	}

	void BaseRenderer::setClearValueColor(Color color)
	{
		m_clearValueColor = color;
	}

	float BaseRenderer::getClearValueDepth() const
	{
		return m_clearValueDepth;
	}

	void BaseRenderer::setClearValueDepth(float value)
	{
		m_clearValueDepth = value;
	}

	uint32_t BaseRenderer::getClearValueStencil() const
	{
		return m_clearValueStencil;
	}

	void BaseRenderer::setClearValueStencil(uint32_t value)
	{
		m_clearValueStencil = value;
	}

	const fd::Rect2D &BaseRenderer::getClearArea() const
	{
		return m_renderArea;
	}

	void BaseRenderer::setClearArea(fd::Rect2D area)
	{
#ifdef DEBUG
		if (area.width < 0)
			throw std::invalid_argument("The width of area is smaller than 0!");
		else if (area.width > 1)
			throw std::invalid_argument("The width of area is bigger than 1!");
		if (area.height < 0)
			throw std::invalid_argument("The height of area is smaller than 0!");
		else if (area.height > 1)
			throw std::invalid_argument("The height of area is bigger than 1!");
		if (area.x < 0)
			throw std::invalid_argument("the x of area is smaller than 0!");
		else if (area.x > area.width)
			throw std::invalid_argument("The x of area is bigger than the width of area!");
		if (area.y < 0)
			throw std::invalid_argument("the y of area is smaller than 0!");
		else if (area.y > area.height)
			throw std::invalid_argument("The y of area is bigger than the height of area!");
#endif // DEBUG

		m_renderArea = area;
	}

	Bool32 BaseRenderer::_isValidForRender()
	{
		return VG_TRUE;
	}

	void BaseRenderer::_render(const RenderInfo &info, RenderResultInfo &resultInfo)
	{
	}

	void BaseRenderer::_createPipelineForRender(std::shared_ptr<vk::Pipeline> &pPipeline,
		std::shared_ptr<BaseMesh> pMesh,
		std::shared_ptr<Material> pMaterial,
		uint32_t subMeshIndex,
		uint32_t passIndex)
	{
		//Create graphics pipeline create info. 
		vk::GraphicsPipelineCreateInfo createInfo = {};

		//Construct shader stage create info.
		auto pPass = pMaterial->getPassWithIndex(passIndex);
		auto pShader = pPass->_getShader();
		auto shaderStages = pShader->getShaderStageInfos();

		createInfo.stageCount = shaderStages.size();
		createInfo.pStages = shaderStages.data();

		const VertexData::SubVertexData &subVertexData = pMesh->getVertexData().getSubVertexDatas()[0];
		const IndexData::SubIndexData &subIndexData = pMesh->getIndexData().getSubIndexDatas()[subMeshIndex];

		createInfo.pVertexInputState = &subVertexData.vertexInputStateInfo;
		createInfo.pInputAssemblyState = &subIndexData.inputAssemblyStateInfo;

        auto cullMode = tranCullModeFlagsToVK(pPass->getCullMode());
		auto frontFace = tranFrontFaceTypeToVK(pPass->getFrontFace());
		//Rasterization info.
		vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
			vk::PipelineRasterizationStateCreateFlags(),  //flags
			VK_FALSE,                                     //depthClampEnable
			VK_FALSE,                                     //rasterizerDiscardEnable
			vk::PolygonMode::eFill,                       //polygonMode
			cullMode,                                     //cullMode
			frontFace,                                    //frontFace
			VK_FALSE,                                     //depthBiasEnable
			0.0f,                                         //depthBiasConstantFactor
			0.0f,                                         //depthBiasClamp
			0.0f,                                         //depthBiasSlopeFactor
			1.0f                                          //lineWidth
		};
		createInfo.pRasterizationState = &rasterizationStateCreateInfo;

		auto depthStencilStateInfoOfPass = pPass->getDepthStencilStateInfo();

		//depth and stencil info.
		createInfo.pDepthStencilState = &depthStencilStateInfoOfPass;

		const auto& colorBlendInfoOfPass = pPass->getColorBlendInfo();
		//color blend info
		vk::PipelineColorBlendAttachmentState defaultColorBlendAttachmentState = {
			VK_FALSE,                                //blendEnable
			vk::BlendFactor::eOne,                  //srcColorBlendFactor
			vk::BlendFactor::eZero,                  //dstColorBlendFactor
			vk::BlendOp::eAdd,                       //colorBlendOp
			vk::BlendFactor::eOne,                  //srcAlphaBlendFactor
			vk::BlendFactor::eZero,                  //desAlphaBlendFactor
			vk::BlendOp::eAdd,                       //alphaBlendOp
			vk::ColorComponentFlagBits::eR
			| vk::ColorComponentFlagBits::eG
			| vk::ColorComponentFlagBits::eB
			| vk::ColorComponentFlagBits::eA  //colorWriteMask
		};

		uint32_t attachmentCount = 1u;
		std::vector<vk::PipelineColorBlendAttachmentState> colorBlendAttachmentStates(attachmentCount);
		for (uint32_t i = 0; i < attachmentCount; ++i)
		{
			if (i < colorBlendInfoOfPass.attachmentCount)
			{
				colorBlendAttachmentStates[i] = *(colorBlendInfoOfPass.pAttachments + i);
			}
			else
			{
				colorBlendAttachmentStates[i] = defaultColorBlendAttachmentState;
			}
		}

		vk::PipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = colorBlendInfoOfPass;
		colorBlendStateCreateInfo.attachmentCount = colorBlendAttachmentStates.size();
		colorBlendStateCreateInfo.pAttachments = colorBlendAttachmentStates.data();
		createInfo.pColorBlendState = &colorBlendStateCreateInfo;

		vk::PipelineViewportStateCreateInfo viewportStateCreateInfo = {
			vk::PipelineViewportStateCreateFlags(),                  //flags
			1u,                                                      //viewportCount
			nullptr,                                               //pViewports
			1u,                                                      //scissorCount
		    nullptr                                                 //pScissors
		};
		createInfo.pViewportState = &viewportStateCreateInfo;

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

		std::vector<vk::DynamicState> dynamicStates = {
			vk::DynamicState::eViewport,
			vk::DynamicState::eScissor
		};

		vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
			vk::PipelineDynamicStateCreateFlags(),
            dynamicStates.size(),
			dynamicStates.data()
		};
		createInfo.pDynamicState = &dynamicStateCreateInfo;		

		createInfo.layout = *pPass->_getPipelineLayout();

		createInfo.renderPass = *m_pRenderPass;
		createInfo.subpass = 0;
		createInfo.basePipelineHandle = nullptr;
		createInfo.basePipelineIndex = -1;
        
		auto pDevice = pApp->getDevice();
		pPipeline = fd::createGraphicsPipeline(pDevice, *m_pPipelineCache, createInfo);
	}

	void BaseRenderer::_recordCommandBufferForRender(std::shared_ptr<vk::Pipeline> pPipeline,
		std::shared_ptr<BaseMesh> pMesh,
		std::shared_ptr<Material> pMaterial,
		uint32_t subMeshIndex,
		uint32_t passIndex)
	{
		LOG(plog::debug) << "Pre begin command buffer for render." << std::endl;

		auto pPass = pMaterial->getPassWithIndex(passIndex);
		
		vk::CommandBufferBeginInfo beginInfo = {
			vk::CommandBufferUsageFlagBits::eSimultaneousUse,  //flags
			nullptr                                            //pInheritanceInfo
		};

		m_pCommandBuffer->begin(beginInfo);
		LOG(plog::debug) << "Post begin command buffer for render." << std::endl;

		vk::ClearValue clearValueColor = {
			std::array<float, 4>{m_clearValueColor[0]
					, m_clearValueColor[1]
					, m_clearValueColor[2]
					, m_clearValueColor[3]
			}
		};
		vk::ClearValue clearValueDepthStencil = {
			vk::ClearDepthStencilValue(m_clearValueDepth
				, m_clearValueStencil
			)
		};
		std::array<vk::ClearValue, 2> clearValues = { clearValueColor
			, clearValueDepthStencil
		};


		const auto& renderArea = m_renderArea;

		vk::RenderPassBeginInfo renderPassBeginInfo = {
			*m_pRenderPass,                                   //renderPass
			*m_pFrameBuffer,                                  //framebuffer
			vk::Rect2D(                                       //renderArea
				vk::Offset2D(static_cast<int32_t>(std::round(m_framebufferWidth * renderArea.x))
					, static_cast<int32_t>(std::round(m_framebufferHeight * renderArea.y))
				),
				vk::Extent2D(static_cast<uint32_t>(std::round(m_framebufferWidth * renderArea.width)),
					static_cast<uint32_t>(std::round(m_framebufferHeight * renderArea.height))
				)
			),
			static_cast<uint32_t>(clearValues.size()),      //clearValueCount
			clearValues.data()                              //pClearValues
		};

		m_pCommandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
        
        const auto& viewportOfPass = pPass->getViewport();
		const auto& scissorOfPass = pPass->getScissor();
		//View port info.
		vk::Viewport viewport = {
			(float)m_framebufferWidth * viewportOfPass.x,                                     //x
			(float)m_framebufferHeight * viewportOfPass.y,                                     //y
			(float)m_framebufferWidth * viewportOfPass.width,   //width
			(float)m_framebufferHeight * viewportOfPass.height,  //height
			1.0f * viewportOfPass.minDepth,                                     //minDepth
			1.0f * viewportOfPass.maxDepth                                      //maxDepth
		};

		m_pCommandBuffer->setViewport(0, viewport);

		vk::Rect2D scissor = {
			{                               //offset
				static_cast<int32_t>(std::round(m_framebufferWidth * scissorOfPass.x)),    //x
				static_cast<int32_t>(std::round(m_framebufferHeight * scissorOfPass.y))    //y
			},
			{                               //extent
				static_cast<uint32_t>(std::round(m_framebufferWidth * scissorOfPass.width)),   //width
				static_cast<uint32_t>(std::round(m_framebufferHeight * scissorOfPass.height))  //height
			}
		};

		m_pCommandBuffer->setScissor(0, scissor);

		m_pCommandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, *pPipeline);

		uint32_t descriptSetCount = pPass->_getDescriptorSet() != nullptr ? 1 : 0;
		std::vector<vk::DescriptorSet> descriptorSets(descriptSetCount);
		if (pPass->_getDescriptorSet() != nullptr)
		{
			descriptorSets[0] = *pPass->_getDescriptorSet();
		}
		auto pPipelineLayout = pPass->_getPipelineLayout();
		m_pCommandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, *pPipelineLayout, 0u, descriptorSets, nullptr);

        vertexDataToCommandBuffer(pMesh->getVertexData(), *m_pCommandBuffer, 0);
		indexDataToCommandBuffer(pMesh->getIndexData(), *m_pCommandBuffer, subMeshIndex);
		m_pCommandBuffer->drawIndexed(pMesh->getIndexData().getSubIndexDatas()[subMeshIndex].indexCount, 1u, 0u, 0u, 0u);
		//m_pCommandBuffer->draw(3, 1, 0, 0);

		m_pCommandBuffer->endRenderPass();

		LOG(plog::debug) << "Pre end command buffer." << std::endl;
		m_pCommandBuffer->end();
		LOG(plog::debug) << "Post end command buffer." << std::endl;
	}

	void BaseRenderer::_init()
	{
		_createRenderPass();
		_createDepthStencilTex();
		_createFramebuffer();
		_createCommandPool();
		_createCommandBuffer();
		_createPipelineCache();
		//_allocateGraphicsQueue();
	}

	void BaseRenderer::_createRenderPass()
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
			m_pColorTexture == nullptr ? vk::ImageLayout::ePresentSrcKHR : vk::ImageLayout::eShaderReadOnlyOptimal     //finalLayout
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
			1u,
			&subpass,
			1u,
			&dependency
		};

		auto pDevice = pApp->getDevice();
		m_pRenderPass = fd::createRenderPass(pDevice, createInfo);
	}

	void BaseRenderer::_createDepthStencilTex()
	{
		m_pDepthStencilTexture = std::shared_ptr<TextureDepthStencilAttachment>(
			new TextureDepthStencilAttachment(
				TextureFormat::D32_SFLOAT_S8_UINT,
				m_framebufferWidth,
				m_framebufferHeight
			)
			);
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
			vk::FramebufferCreateFlags(),                   //flags
			*m_pRenderPass,                                 //renderPass
			static_cast<uint32_t>(attachments.size()),      //attachmentCount
			attachments.data(),                             //pAttachments
			m_framebufferWidth,                             //width
			m_framebufferHeight,                            //height
			1u                                              //layers
		};

		auto pDevice = pApp->getDevice();
		m_pFrameBuffer = fd::createFrameBuffer(pDevice, createInfo);
	}

	void BaseRenderer::_createCommandPool()
	{
		auto pDevice = pApp->getDevice();
		auto graphicsFamily = pApp->getGraphicsFamily();
		vk::CommandPoolCreateInfo createInfo = {
			vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
			graphicsFamily
		};
		m_pCommandPool = fd::createCommandPool(pDevice, createInfo);
	}

	void BaseRenderer::_createCommandBuffer()
	{
		auto pCommandPool = m_pCommandPool;
		vk::CommandBufferAllocateInfo allocateInfo = {
			*pCommandPool,                             //commandPool
			vk::CommandBufferLevel::ePrimary,          //level
			1u                                         //commandBufferCount
		};

		auto pDevice = pApp->getDevice();

		LOG(plog::debug) << "Pre allocate command buffer from pool." << std::endl;
		m_pCommandBuffer = fd::allocateCommandBuffer(pDevice, pCommandPool, allocateInfo);
		LOG(plog::debug) << "Post allocate command buffer from pool." << std::endl;
	}

	void BaseRenderer::_createPipelineCache()
	{
		vk::PipelineCacheCreateInfo createInfo = {
			vk::PipelineCacheCreateFlags()
		};
		auto pDevice = pApp->getDevice();
		m_pPipelineCache = fd::createPipelineCache(pDevice, createInfo);
	}

	/*void BaseRenderer::_allocateGraphicsQueue()
	{
		pApp->allocateGaphicsQueue(m_graphicsQueueIndex, m_graphicsQueue);
	}

	void BaseRenderer::_freeGraphicsQueue()
	{
		pApp->freeGraphicsQueue(m_graphicsQueueIndex);
	}*/
}