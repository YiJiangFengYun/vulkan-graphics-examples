#include "graphics/renderer/renderer.hpp"

#include "graphics/vertex_data/util.hpp"

namespace vg
{
	void fillValidVisualObjects(std::vector<std::shared_ptr<VisualObject<SpaceType::SPACE_2>>> &arrPVObjs,
		uint32_t &PVObjIndex,
		const Transform<SpaceType::SPACE_2> *pTransform,
		const Scene<SpaceType::SPACE_2> *pScene,
	    const Camera<SpaceType::SPACE_2> *pCamera);

	const vk::Format Renderer::DEFAULT_DEPTH_STENCIL_FORMAT(vk::Format::eD32SfloatS8Uint);

	Renderer::Renderer(std::shared_ptr<vk::ImageView> pSwapchainImageView
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
		, m_pipelineCache()
	{
		_init();
	}

	Renderer::Renderer(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex)
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
		, m_pipelineCache()
	{
		_init();
	}

	Renderer::~Renderer()
	{
		//_freeGraphicsQueue();
	}

	Bool32 Renderer::isValidForRender() const
	{
		return _isValidForRender();
	}

	void Renderer::render(const RenderInfo &info, 
		RenderResultInfo &resultInfo)
	{
		_preRender();
		_render(info, resultInfo);
		_postRender();
	}

	uint32_t Renderer::getFramebufferWidth() const
	{
		return m_framebufferWidth;
	}

	uint32_t Renderer::getFramebufferHeight() const
	{
		return m_framebufferHeight;
	}

	vk::Format Renderer::getColorImageFormat() const
	{
		return m_colorImageFormat;
	}

	vk::Format Renderer::getDepthStencilImageFormat() const
	{
		return m_depthStencilImageFormat;
	}

	const Color &Renderer::getClearValueColor() const
	{
		return m_clearValueColor;
	}

	void Renderer::setClearValueColor(Color color)
	{
		m_clearValueColor = color;
	}

	float Renderer::getClearValueDepth() const
	{
		return m_clearValueDepth;
	}

	void Renderer::setClearValueDepth(float value)
	{
		m_clearValueDepth = value;
	}

	uint32_t Renderer::getClearValueStencil() const
	{
		return m_clearValueStencil;
	}

	void Renderer::setClearValueStencil(uint32_t value)
	{
		m_clearValueStencil = value;
	}

	const fd::Rect2D &Renderer::getClearArea() const
	{
		return m_renderArea;
	}

	void Renderer::setClearArea(fd::Rect2D area)
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

	Bool32 Renderer::_isValidForRender() const
	{
		return VG_TRUE;
	}

	void Renderer::_preRender()
	{
		m_pipelineCache.start();
	}

	void Renderer::_render(const RenderInfo &info
		, RenderResultInfo &resultInfo)
	{
		resultInfo.signalSemaphoreCount = 0u;
		uint32_t count = info.sceneAndCameraCount;
		for (uint32_t i = 0; i < count; ++i)
		{
			resultInfo.isRendered = VG_TRUE;
			const auto &pScene = (*(info.pSceneAndCamera + i)).pScene;
			const auto &pCamera = (*(info.pSceneAndCamera + i)).pCamera;

			if (pScene->getSpaceType() == SpaceType::SPACE_2)
			{
				_renderScene2(dynamic_cast<const Scene<SpaceType::SPACE_2> *>(pScene), 
				    dynamic_cast<const Camera<SpaceType::SPACE_2> *>(pCamera), 
					info, 
					resultInfo);
			} else if (pScene->getSpaceType() == SpaceType::SPACE_3)
			{
				_renderScene3(dynamic_cast<const Scene<SpaceType::SPACE_3> *>(pScene), 
				    dynamic_cast<const Camera<SpaceType::SPACE_3> *>(pCamera), 
					info, 
					resultInfo);
			} else {
				//todo
			}
		}
	}

	void Renderer::_postRender()
	{
		m_pipelineCache.end();
	}

	void Renderer::_createPipelineForRender(std::shared_ptr<vk::Pipeline> &pPipeline,
		std::shared_ptr<BaseMesh> pMesh,
		std::shared_ptr<Material> pMaterial,
		uint32_t subMeshIndex,
		uint32_t passIndex)
	{
		ContentMesh * pContentMesh = dynamic_cast<ContentMesh *>(pMesh.get());
		auto pPass = pMaterial->getPassWithIndex(passIndex);
		PipelineCache::Info info(
			*m_pRenderPass,
			pPass,
			pContentMesh->getVertexData(),
			0u,
			pContentMesh->getIndexData(),
			subMeshIndex
		);
		pPipeline = m_pipelineCache.caching(info);
	}

	void Renderer::_recordCommandBufferForRender(std::shared_ptr<vk::Pipeline> pPipeline,
		std::shared_ptr<BaseMesh> pMesh,
		std::shared_ptr<Material> pMaterial,
		uint32_t subMeshIndex,
		uint32_t passIndex)
	{
		LOG(plog::debug) << "Pre begin command buffer for render." << std::endl;
		ContentMesh * pContentMesh = dynamic_cast<ContentMesh *>(pMesh.get());
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
			(float)m_framebufferWidth * viewportOfPass.x,                      //x
			(float)m_framebufferHeight * viewportOfPass.y,                     //y
			(float)m_framebufferWidth * viewportOfPass.width,                  //width
			(float)m_framebufferHeight * viewportOfPass.height,                 //height
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

		auto pPipelineLayout = pPass->getPipelineLayout();		

		//push constants
		auto pushConstantUpdates = pPass->getPushconstantUpdates();
		for(const auto& pPushConstantUpdate : pushConstantUpdates)
		{
			m_pCommandBuffer->pushConstants(*pPipelineLayout, 
			    pPushConstantUpdate->getStageFlags(), 
				pPushConstantUpdate->getOffset(),
				pPushConstantUpdate->getSize(),
				pPushConstantUpdate->getData());
		}


		m_pCommandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, *pPipeline);

		uint32_t descriptSetCount = pPass->getDescriptorSet() != nullptr ? 1 : 0;
		std::vector<vk::DescriptorSet> descriptorSets(descriptSetCount);
		if (pPass->getDescriptorSet() != nullptr)
		{
			descriptorSets[0] = *pPass->getDescriptorSet();
		}
		m_pCommandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, *pPipelineLayout, 0u, descriptorSets, nullptr);

        vertexDataToCommandBuffer(*m_pCommandBuffer, pContentMesh->getVertexData(), 0);
		indexDataToCommandBuffer(*m_pCommandBuffer, pContentMesh->getIndexData());
		m_pCommandBuffer->drawIndexed(pContentMesh->getIndexData()->getSubIndexDatas()[subMeshIndex].indexCount, 1u, 0u, 0u, 0u);
		//m_pCommandBuffer->draw(3, 1, 0, 0);

		m_pCommandBuffer->endRenderPass();

		LOG(plog::debug) << "Pre end command buffer." << std::endl;
		m_pCommandBuffer->end();
		LOG(plog::debug) << "Post end command buffer." << std::endl;
	}

	void Renderer::_init()
	{
		_createRenderPass();
		_createDepthStencilTex();
		_createFramebuffer();
		_createCommandPool();
		_createCommandBuffer();
	}

	void Renderer::_createRenderPass()
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

	void Renderer::_createDepthStencilTex()
	{
		m_pDepthStencilTexture = std::shared_ptr<TextureDepthStencilAttachment>(
			new TextureDepthStencilAttachment(
				TextureFormat::D32_SFLOAT_S8_UINT,
				m_framebufferWidth,
				m_framebufferHeight
			)
			);
	}

	void Renderer::_createFramebuffer()
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

	void Renderer::_createCommandPool()
	{
		auto pDevice = pApp->getDevice();
		auto graphicsFamily = pApp->getGraphicsFamily();
		vk::CommandPoolCreateInfo createInfo = {
			vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
			graphicsFamily
		};
		m_pCommandPool = fd::createCommandPool(pDevice, createInfo);
	}

	void Renderer::_createCommandBuffer()
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

	void Renderer::_renderScene2(const Scene<SpaceType::SPACE_2> *pScene
		, const Camera<SpaceType::SPACE_2> *pCamera
	    , const RenderInfo &info
	    , RenderResultInfo &resultInfo)
	{
		using SceneType = Scene<SpaceType::SPACE_2>;
		auto pDevice = pApp->getDevice();
		auto projMatrix = pCamera->getProjMatrix();
		auto viewMatrix = pCamera->getTransform()->getMatrixWorldToLocal();
		uint32_t visualObjectCount = pScene->getVisualObjectCount();

		//flat visual objects and filter them that is out of camera with its bounds.
		//allocate enough space for array to storage points.
		std::vector<std::shared_ptr<SceneType::VisualObjectType>> validVisualObjects(visualObjectCount);
		uint32_t validVisualObjectCount(0u);
		auto pRoot = pScene->pRootTransformForVisualObject;
		auto pTransform = pRoot;
		fillValidVisualObjects(validVisualObjects
			, validVisualObjectCount
			, pTransform.get()
			, pScene
			, pCamera
		);

		//Caculate total draw count.
		uint32_t drawCount = 0;
		for (uint32_t i = 0; i < validVisualObjectCount; ++i)
		{
			auto pVisualObject = validVisualObjects[i];
			auto pMesh = pVisualObject->getMesh();
			auto pContentMesh = dynamic_cast<ContentMesh *>(pMesh.get());
			auto subMeshCount = pContentMesh->getIndexData()->getSubIndexDataCount();
			auto pMaterial = pVisualObject->getMaterial();
			auto passCount = pMaterial->getPassCount();
			drawCount += subMeshCount * passCount;
		}

		//------Doing render.
		vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
		std::vector<vk::SubmitInfo> submitInfos(drawCount);
		uint32_t semaphoreCount = resultInfo.signalSemaphoreCount + drawCount;
		uint32_t semaphoreIndex = resultInfo.signalSemaphoreCount;
		if (m_arrCachePSemaphores.size() < semaphoreCount)
		    m_arrCachePSemaphores.resize(semaphoreCount);
		m_arrSemaphores.resize(semaphoreCount);

		uint32_t drawIndex = 0u;
		for (uint32_t i = 0u; i < validVisualObjectCount; ++i)
		{
			auto pVisualObject = validVisualObjects[i];
			auto modelMatrix = pVisualObject->getTransform()->getMatrixLocalToWorld();
			auto mvMatrix = viewMatrix * modelMatrix;
			auto mvpMatrix = projMatrix * mvMatrix;
			auto pMesh = pVisualObject->getMesh();
			auto pContentMesh = dynamic_cast<ContentMesh *>(pMesh.get());
			auto subMeshCount = pContentMesh->getIndexData()->getSubIndexDataCount();
			auto pMaterial = pVisualObject->getMaterial();
			auto passCount = pMaterial->getPassCount();
			for (uint32_t passIndex = 0u; passIndex < passCount; ++passIndex)
			{
				//update building in matrix variable.
				auto pPass = pMaterial->getPassWithIndex(passIndex);

				pPass->_setBuildInMatrixData(mvpMatrix, mvMatrix, modelMatrix);
				pPass->apply();
			}

			for (uint32_t subMeshIndex = 0u; subMeshIndex < subMeshCount; ++subMeshIndex)
			{
				for (uint32_t passIndex = 0u; passIndex < passCount; ++passIndex)
				{
					std::shared_ptr<vk::Pipeline> pPipeline;
					_createPipelineForRender(pPipeline, pMesh, pMaterial, subMeshIndex, passIndex);
					_recordCommandBufferForRender(pPipeline, pMesh, pMaterial, subMeshIndex, passIndex);

					//submit
					std::shared_ptr<vk::Semaphore> pSemaphore = nullptr;
					if (m_arrCachePSemaphores[semaphoreIndex] != nullptr)
					{
						pSemaphore = m_arrCachePSemaphores[semaphoreIndex];
					}
					else
					{
						vk::SemaphoreCreateInfo createInfo = {
							vk::SemaphoreCreateFlags()
						};
						pSemaphore = fd::createSemaphore(pDevice, createInfo);
						m_arrCachePSemaphores[semaphoreIndex] = pSemaphore;
					}
					m_arrSemaphores[semaphoreIndex] = *pSemaphore;

					vk::SubmitInfo submitInfo = {
						info.waitSemaphoreCount,              //waitSemaphoreCount
						info.pWaitSemaphores,                 //pWaitSemaphores
						waitStages,                           //pWaitDstStageMask
						1u,                                   //commandBufferCount
						m_pCommandBuffer.get(),               //pCommandBuffers
						1u,                                   //signalSemaphoreCount
						pSemaphore.get()                      //pSignalSemaphores
					};

					submitInfos[drawIndex] = submitInfo;
					++drawIndex;
					++semaphoreIndex;
				}
			}
		}

		LOG(plog::debug) << "Pre submit to grahics queue." << std::endl;
		vk::Queue queue;
		uint32_t queueIndex;
		pApp->allocateGaphicsQueue(queueIndex, queue);
		queue.submit(submitInfos, nullptr);
		pApp->freeGraphicsQueue(queueIndex);
		LOG(plog::debug) << "Post submit to grahics queue." << std::endl;

		resultInfo.signalSemaphoreCount = static_cast<uint32_t>(m_arrSemaphores.size());
		resultInfo.pSignalSemaphores = m_arrSemaphores.data();
	}

	void Renderer::_renderScene3(const Scene<SpaceType::SPACE_3> *pScene
		, const Camera<SpaceType::SPACE_3> *pCamera
	    , const RenderInfo &info
	    , RenderResultInfo &resultInfo)
	{
		using SceneType = Scene<SpaceType::SPACE_3>;

		auto queueTypeCount = static_cast<uint32_t>(RenderQueueType::RANGE_SIZE);
		auto pDevice = pApp->getDevice();

		auto projMatrix = pCamera->getProjMatrix();
		auto viewMatrix = pCamera->getTransform()->getMatrixWorldToLocal();

		uint32_t visualObjectCount = pScene->getVisualObjectCount();
		
		//----------Preparing render.

		//Filter visualObject is out of camera with its bounds.
		std::vector<std::shared_ptr<SceneType::VisualObjectType>> validVisualObjects(visualObjectCount); //allocate enough space for array to storage points.
		uint32_t validVisualObjectCount(0u);
		for (uint32_t i = 0; i < visualObjectCount; ++i)
		{
			auto pVisualObject = pScene->getVisualObjectWithIndex(i);
			auto pMesh = pVisualObject->getMesh();
			auto isHasBounds = dynamic_cast<SceneType::VisualObjectType::MeshDimType *>(pMesh.get())->getIsHasBounds();
			if (isHasBounds == VG_FALSE)
			{
				validVisualObjects[validVisualObjectCount++] = pVisualObject;
			}
			else 
			{
			    auto bounds = dynamic_cast<SceneType::VisualObjectType::MeshDimType *>(pMesh.get())->getBounds();
			    auto pTransform = pVisualObject->getTransform();
			    if(pCamera->isInView(pTransform.get(), bounds) == VG_TRUE)
			    {
			    	validVisualObjects[validVisualObjectCount++] = pVisualObject;
			    }
			}
		}

		//Get queue count for each queue type.
		std::vector<uint32_t> queueLengths(queueTypeCount, 0u);
		for (uint32_t i = 0; i < validVisualObjectCount; ++i)
		{
			auto pVisualObject = validVisualObjects[i];
			auto renderQueueType = tranMaterialShowTypeToRenderQueueType(pVisualObject->getMaterial()->getShowType());
			++queueLengths[static_cast<size_t>(renderQueueType)];
		}

		std::vector<std::vector<std::shared_ptr<SceneType::VisualObjectType>>> queues(queueTypeCount);
		//Resize queues and reset quue counts to zero for preparing next use.
		for (uint32_t i = 0; i < queueTypeCount; ++i)
		{
			queues[i].resize(queueLengths[i], nullptr);
			queueLengths[i] = 0u;
		}

		//Update queues to point to visual object.
		for (uint32_t i = 0; i < validVisualObjectCount; ++i)
		{
			auto pVisualObject = validVisualObjects[i];
			auto renderQueueType = tranMaterialShowTypeToRenderQueueType(pVisualObject->getMaterial()->getShowType());
			queues[static_cast<size_t>(renderQueueType)][queueLengths[static_cast<size_t>(renderQueueType)]++] = pVisualObject;
		}

		//sort transparent queue.
		std::sort(queues[static_cast<size_t>(RenderQueueType::TRANSPARENT)].begin(),
			queues[static_cast<size_t>(RenderQueueType::TRANSPARENT)].end(),
			[&viewMatrix, &projMatrix](std::shared_ptr<typename SceneType::ObjectType> pObject1, std::shared_ptr<typename SceneType::ObjectType> pObject2)
	        {
				auto modelMatrix1 = pObject1->getTransform()->getMatrixLocalToWorld();
				auto mvMatrix1 = viewMatrix * modelMatrix1;
				auto modelMatrix2 = pObject2->getTransform()->getMatrixLocalToWorld();
				auto mvMatrix2 = viewMatrix * modelMatrix2;
	        	//get position
	        	auto pos1 = pObject1->getTransform()->getLocalPosition();
	        	auto pos2 = pObject2->getTransform()->getLocalPosition();
        
	        	typedef SpaceTypeInfo<SpaceType::SPACE_3>::PointType PointType;
	        	typedef SpaceTypeInfo<SpaceType::SPACE_3>::MatrixVectorType MatrixVectorType;
	        	//transform point from model coordinate system to camera coordinate system.
	        	pos1 = mvMatrix1 * MatrixVectorType(pos1, 1.0f);
	        	pos2 = mvMatrix2 * MatrixVectorType(pos2, 1.0f);
        
	        	//it is smaller if its z is bigger than the other.
	        	return static_cast<Bool32>(pos1.z > pos2.z);
        
	        });

		//Caculate total draw count.
		uint32_t drawCount = 0;
		for (uint32_t typeIndex = 0u; typeIndex < queueTypeCount; ++typeIndex)
		{
			auto queueLength = queueLengths[typeIndex];
			for (uint32_t objectIndex = 0u; objectIndex < queueLength; ++objectIndex)
			{
				auto pVisualObject = queues[typeIndex][objectIndex];
				auto pMesh = pVisualObject->getMesh();
				auto pContentMesh = dynamic_cast<ContentMesh *>(pMesh.get());
				auto subMeshCount = pContentMesh->getIndexData()->getSubIndexDataCount();
				auto pMaterial = pVisualObject->getMaterial();
				auto passCount = pMaterial->getPassCount();
				drawCount += subMeshCount * passCount;
			}
		}

		//-----Doing render.
		
		vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
		std::vector<vk::SubmitInfo> submitInfos(drawCount);
		uint32_t semaphoreCount = resultInfo.signalSemaphoreCount + drawCount;
		uint32_t semaphoreIndex = resultInfo.signalSemaphoreCount;
		if (m_arrCachePSemaphores.size() < semaphoreCount)
		    m_arrCachePSemaphores.resize(semaphoreCount);
		m_arrSemaphores.resize(semaphoreCount);

		uint32_t drawIndex = 0;
		for (uint32_t typeIndex = 0u; typeIndex < queueTypeCount; ++typeIndex)
		{
			auto queueLength = queueLengths[typeIndex];
			for (uint32_t objectIndex = 0u; objectIndex < queueLength; ++objectIndex)
			{
				auto pVisualObject = queues[typeIndex][objectIndex];
				auto modelMatrix = pVisualObject->getTransform()->getMatrixLocalToWorld();
				auto mvMatrix = viewMatrix * modelMatrix;
				auto mvpMatrix = projMatrix * mvMatrix;
				auto pMesh = pVisualObject->getMesh();
				auto pContentMesh = dynamic_cast<ContentMesh *>(pMesh.get());
				auto subMeshCount = pContentMesh->getSubMeshCount();
				auto pMaterial = pVisualObject->getMaterial();
				auto passCount = pMaterial->getPassCount();

				for (uint32_t passIndex = 0u; passIndex < passCount; ++passIndex)
				{
					//update building in matrix variable.
					auto pPass = pMaterial->getPassWithIndex(passIndex);

					pPass->_setBuildInMatrixData(mvpMatrix, mvMatrix, modelMatrix);

					pPass->apply();
				}

				for (uint32_t subMeshIndex = 0u; subMeshIndex < subMeshCount; ++subMeshIndex)
				{
					for (uint32_t passIndex = 0u; passIndex < passCount; ++passIndex)
					{
						std::shared_ptr<vk::Pipeline> pPipeline;
						_createPipelineForRender(pPipeline, pMesh, pMaterial, subMeshIndex, passIndex);
						_recordCommandBufferForRender(pPipeline, pMesh, pMaterial, subMeshIndex, passIndex);

						//submit
						std::shared_ptr<vk::Semaphore> pSemaphore = nullptr;
						if (m_arrCachePSemaphores[semaphoreIndex] != nullptr)
						{
							pSemaphore = m_arrCachePSemaphores[semaphoreIndex];
						}
						else
						{
							vk::SemaphoreCreateInfo createInfo = {
								vk::SemaphoreCreateFlags()
							};
							pSemaphore = fd::createSemaphore(pDevice, createInfo);
							m_arrCachePSemaphores[semaphoreIndex] = pSemaphore;
						}
						m_arrSemaphores[semaphoreIndex] = *pSemaphore;

						vk::SubmitInfo submitInfo = {
							info.waitSemaphoreCount,              //waitSemaphoreCount
							info.pWaitSemaphores,                 //pWaitSemaphores
							waitStages,                           //pWaitDstStageMask
							1u,                                   //commandBufferCount
							m_pCommandBuffer.get(),               //pCommandBuffers
							1u,                                   //signalSemaphoreCount
							pSemaphore.get()                      //pSignalSemaphores
						};

						submitInfos[drawIndex] = submitInfo;
						++drawIndex;
						++semaphoreIndex;
					}
				}
			}
		}

		LOG(plog::debug) << "Pre submit to grahics queue." << std::endl;
		vk::Queue queue;
		uint32_t queueIndex;
		pApp->allocateGaphicsQueue(queueIndex, queue);
		queue.submit(submitInfos, nullptr);
		pApp->freeGraphicsQueue(queueIndex);
		LOG(plog::debug) << "Post submit to grahics queue." << std::endl;

		resultInfo.signalSemaphoreCount = static_cast<uint32_t>(m_arrSemaphores.size());
		resultInfo.pSignalSemaphores = m_arrSemaphores.data();
	}

	void fillValidVisualObjects(std::vector<std::shared_ptr<VisualObject<SpaceType::SPACE_2>>> &arrPVObjs,
		uint32_t &PVObjIndex,
		const Transform<SpaceType::SPACE_2> *pTransform,
		const Scene<SpaceType::SPACE_2> *pScene,
	    const Camera<SpaceType::SPACE_2> *pCamera)
	{
		std::shared_ptr<VisualObject<SpaceType::SPACE_2>> pVisualObjectOfChild;
		uint32_t childCount = pTransform->getChildCount();
		Transform<SpaceType::SPACE_2> *pChild;
		for (uint32_t i = 0; i < childCount; ++i)
		{
			pChild = pTransform->getChildWithIndex(i);
			//Children's visual object is placed ahead of own visual objects
			fillValidVisualObjects(arrPVObjs, PVObjIndex, pChild, pScene, pCamera);
			//Own visual object is placed behind children's visual object.
			pVisualObjectOfChild = pScene->getVisualObjectWithTransform(pChild);
			//Filter obj out of camera view.
			auto pMeshOfChild = pVisualObjectOfChild->getMesh();
			auto isHasBoundsOfChild = dynamic_cast<Mesh<MeshDimType::SPACE_2> *>(pMeshOfChild.get())->getIsHasBounds();
			if (isHasBoundsOfChild == VG_FALSE) 
			{
				arrPVObjs[PVObjIndex++] = pVisualObjectOfChild;
			}
			else {
			    auto boundsOfChild = dynamic_cast<Mesh<MeshDimType::SPACE_2> *>(pMeshOfChild.get())->getBounds();
			    if (pCamera->isInView(pChild, boundsOfChild) == VG_TRUE)
			    {
			    	arrPVObjs[PVObjIndex++] = pVisualObjectOfChild;
			    }
			}
		}
	}
}