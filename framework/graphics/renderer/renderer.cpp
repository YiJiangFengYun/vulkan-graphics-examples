#include "graphics/renderer/renderer.hpp"

#include "graphics/buffer_data/util.hpp"
#include "graphics/util/gemo_util.hpp"
#include "graphics/renderer/cmd_parser.hpp"

// #define USE_WORLD_BOUNDS

namespace vg
{
	void fillValidVisualObjects(std::vector<VisualObject<SpaceType::SPACE_2> *> &arrPVObjs
	    , uint32_t &PVObjIndex
		, const Transform<SpaceType::SPACE_2> *pTransform
		, const Scene<SpaceType::SPACE_2> *pScene
		, const Camera<SpaceType::SPACE_2> *pCamera
#ifdef USE_WORLD_BOUNDS
		, const fd::Bounds<SpaceTypeInfo<SpaceType::SPACE_2>::PointType> *pBounds
#endif
		);

	Renderer::RenderInfo::RenderInfo(uint32_t sceneAndCameraCount
		, const SceneAndCamera *pSceneAndCameras
		, uint32_t waitSemaphoreCount
		, const vk::Semaphore* pWaitSemaphores
		, const vk::PipelineStageFlags* pWaitDstStageMask
		, uint32_t signalSemaphoreCount
		, const vk::Semaphore* pSignalSemaphores
		)
		: sceneAndCameraCount(sceneAndCameraCount)
		, pSceneAndCameras(pSceneAndCameras)
		, waitSemaphoreCount(waitSemaphoreCount)
		, pWaitSemaphores(pWaitSemaphores)
		, pWaitDstStageMask(pWaitDstStageMask)
		, signalSemaphoreCount(signalSemaphoreCount)
		, pSignalSemaphores(pSignalSemaphores)
	{
	}

	Renderer::RenderResultInfo::RenderResultInfo(Bool32 isRendered
		, uint32_t drawCount
	    )
		: isRendered(isRendered)
		, drawCount(drawCount)
	{

	}

	const vk::Format Renderer::DEFAULT_DEPTH_STENCIL_FORMAT(vk::Format::eD32SfloatS8Uint);

	Renderer::Renderer()
		: Base(BaseType::RENDERER)
		, m_colorImageFormat(vk::Format::eUndefined)
		, m_depthStencilImageFormat(DEFAULT_DEPTH_STENCIL_FORMAT)
		, m_framebufferWidth(0)
		, m_framebufferHeight(0)
		, m_clearValueColor(0.0f)
		, m_clearValueDepth(1.0f)
		, m_clearValueStencil(0u)
		, m_renderArea(0.0f, 0.0f, 1.0f, 1.0f)
		, m_pipelineCache()
		, m_pCurrRenderPass()
		, m_pCurrFrameBuffer()
		, m_trunkRenderPassCmdBuffer()
		, m_trunkWaitBarrierCmdBuffer()
		, m_branchCmdBuffer()
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        , m_preparingRenderCostTimer(fd::CostTimer::TimerType::AVERAGE)
#endif //DEBUG and VG_ENABLE_COST_TIMER
	{
		_createCommandPool();
		_createCommandBuffer();
		
		//_createFence();
	}

	Renderer::~Renderer()
	{
	}

	Bool32 Renderer::isValidForRender() const
	{
		return _isValidForRender();
	}

	void Renderer::render(const RenderInfo &info, 
		RenderResultInfo &resultInfo)
	{
		_preRender();
		_renderBegin();
		_render(info, resultInfo);
		_renderEnd(info);
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

	// Renderer::PreObjectRecordingFun Renderer::setPreObjectRecordingCallBack(PreObjectRecordingFun cbFun)
	// {
	// 	auto oldFun = m_preObjectRecordingFun;
	// 	m_preObjectRecordingFun = cbFun;
	// 	return oldFun;
	// }
		
	// Renderer::PostObjectRecordingFun Renderer::setPostObjectRecordingCallBack(PostObjectRecordingFun cbFun)
	// {
	// 	auto oldFun = m_postObjectRecordingFun;
	// 	m_postObjectRecordingFun = cbFun;
	// 	return oldFun;
	// }

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
    const fd::CostTimer &Renderer::getPreparingRenderCostTimer() const
	{
		return m_preparingRenderCostTimer;
	}
#endif //DEBUG and VG_ENABLE_COST_TIMER

	Bool32 Renderer::_isValidForRender() const
	{
		return VG_TRUE;
	}

	void Renderer::_preRender()
	{
		m_pipelineCache.start();
	}

	void Renderer::_renderBegin()
	{
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
		m_preparingRenderCostTimer.begin();

		fd::CostTimer renderBeginCostTimer(fd::CostTimer::TimerType::ONCE);
		renderBeginCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
		//command buffer begin
		_recordCommandBufferForBegin();

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
		renderBeginCostTimer.end();
		VG_COST_TIME_LOG(plog::debug) << "Render begin cost time: "
			<< renderBeginCostTimer.costTimer
			<< std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER
	}

	void Renderer::_render(const RenderInfo &info
		    , RenderResultInfo &resultInfo)
	{
		resultInfo.drawCount = 0u;

		m_trunkRenderPassCmdBuffer.begin();
		m_trunkWaitBarrierCmdBuffer.begin();
		m_branchCmdBuffer.begin();
		uint32_t count = info.sceneAndCameraCount;
		for (uint32_t i = 0; i < count; ++i)
		{
			const auto &pScene = (*(info.pSceneAndCameras + i)).pScene;
			const auto &pCamera = (*(info.pSceneAndCameras + i)).pCamera;
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
            fd::CostTimer preparingSceneCostTimer(fd::CostTimer::TimerType::ONCE);
			preparingSceneCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
            
			if (pScene->getSpaceType() == SpaceType::SPACE_2)
			{
				_renderScene2(dynamic_cast<const Scene<SpaceType::SPACE_2> *>(pScene), 
				    dynamic_cast<const Camera<SpaceType::SPACE_2> *>(pCamera), 
					info,
					&m_branchCmdBuffer,					
					&m_trunkRenderPassCmdBuffer,
					&m_trunkWaitBarrierCmdBuffer
					);
			} else if (pScene->getSpaceType() == SpaceType::SPACE_3)
			{
				_renderScene3(dynamic_cast<const Scene<SpaceType::SPACE_3> *>(pScene), 
				    dynamic_cast<const Camera<SpaceType::SPACE_3> *>(pCamera), 
					info, 
					&m_branchCmdBuffer,					
					&m_trunkRenderPassCmdBuffer,
					&m_trunkWaitBarrierCmdBuffer
					);
			} else {
				//todo
			}

			resultInfo.isRendered = VG_TRUE;

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
			preparingSceneCostTimer.end();
			VG_COST_TIME_LOG(plog::debug) << "Preparing scene cost time: " 
			    << preparingSceneCostTimer.costTimer 
				<< "ms, scene id: " << pScene->getID() 
				<< ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
				<<  std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER
		}

		//branch render pass.
		CMDParser::recordBranch(&m_branchCmdBuffer,
			m_pCommandBuffer.get(),
			&m_pipelineCache
			);

		//trunk wait barrier
		CMDParser::recordTrunkWaitBarrier(&m_trunkWaitBarrierCmdBuffer,
			m_pCommandBuffer.get());

        //trunk render pass.
		resultInfo.drawCount = m_trunkRenderPassCmdBuffer.getCmdCount();
		_recordTrunkRenderPassForBegin();
		CMDParser::recordTrunk(&m_trunkRenderPassCmdBuffer,
			m_pCommandBuffer.get(),
			&m_pipelineCache,
			m_pCurrRenderPass
			);
		_recordTrunkRenderPassForEnd();
	}

	void Renderer::_renderEnd(const RenderInfo &info)
	{

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        fd::CostTimer renderEndCostTimer(fd::CostTimer::TimerType::ONCE);
		renderEndCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER	

		//command buffer end
		_recordCommandBufferForEnd();

		/*auto pDevice = pApp->getDevice();
		pDevice->waitForFences(*m_waitFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
		pDevice->resetFences(*m_waitFence);*/
		

		vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };		
		//submit		
		vk::SubmitInfo submitInfo = {
			info.waitSemaphoreCount,              //waitSemaphoreCount
			info.pWaitSemaphores,                 //pWaitSemaphores
			waitStages,                           //pWaitDstStageMask
			1u,                                   //commandBufferCount
			m_pCommandBuffer.get(),               //pCommandBuffers
			info.signalSemaphoreCount,            //signalSemaphoreCount
			info.pSignalSemaphores,               //pSignalSemaphores
		};

		VG_LOG(plog::debug) << "Pre submit to grahics queue." << std::endl;
		vk::Queue queue;
		uint32_t queueIndex;
		pApp->allocateGaphicsQueue(queueIndex, queue);
		queue.submit(submitInfo, nullptr);
		//queue.submit(submitInfo, *m_waitFence);
		pApp->freeGraphicsQueue(queueIndex);
		VG_LOG(plog::debug) << "Post submit to grahics queue." << std::endl;
		
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        renderEndCostTimer.end();
		VG_COST_TIME_LOG(plog::debug) << "Render end cost time: " 
			    << renderEndCostTimer.costTimer 
				<< std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
		m_preparingRenderCostTimer.end();
		VG_COST_TIME_LOG(plog::debug) << "Average cost time of preparing render: " << m_preparingRenderCostTimer.costTimer << "ms." << std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER
	}

	void Renderer::_postRender()
	{
		m_pipelineCache.end();
	}

	void Renderer::_recordCommandBufferForBegin()
	{
		vk::CommandBufferBeginInfo beginInfo = {
			vk::CommandBufferUsageFlagBits::eSimultaneousUse,  //flags
			nullptr                                            //pInheritanceInfo
		};

		m_pCommandBuffer->begin(beginInfo);
		VG_LOG(plog::debug) << "Post begin command buffer for render." << std::endl;

		
	}

	void Renderer::_recordTrunkRenderPassForBegin()
	{
		vk::ClearValue clearValueColor = {
			std::array<float, 4>{
			    m_clearValueColor[0], 
				m_clearValueColor[1], 
				m_clearValueColor[2], 
				m_clearValueColor[3]
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
			*m_pCurrRenderPass,                                   //renderPass
			*m_pCurrFrameBuffer,                                  //framebuffer
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
	}

	void Renderer::_recordTrunkRenderPassForEnd()
    {
		m_pCommandBuffer->endRenderPass();
	}

	void Renderer::_recordCommandBufferForEnd()
	{
		
		VG_LOG(plog::debug) << "Pre end command buffer." << std::endl;
		m_pCommandBuffer->end();
		VG_LOG(plog::debug) << "Post end command buffer." << std::endl;
	}

	

	/*void Renderer::_createFence()
	{
		if (m_waitFence != nullptr) return;
		auto pDevice = pApp->getDevice();
		vk::FenceCreateInfo createInfo;
		createInfo.flags = vk::FenceCreateFlagBits::eSignaled;
		m_waitFence = fd::createFence(pDevice, createInfo);
	}*/

	void Renderer::_createCommandPool()
	{
		if (m_pCommandPool != nullptr) return;
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
		if (m_pCommandBuffer != nullptr) return;
		auto pCommandPool = m_pCommandPool;
		vk::CommandBufferAllocateInfo allocateInfo = {
			*pCommandPool,                             //commandPool
			vk::CommandBufferLevel::ePrimary,          //level
			1u                                         //commandBufferCount
		};

		auto pDevice = pApp->getDevice();

		VG_LOG(plog::debug) << "Pre allocate command buffer from pool." << std::endl;
		m_pCommandBuffer = fd::allocateCommandBuffer(pDevice, pCommandPool.get(), allocateInfo);
		VG_LOG(plog::debug) << "Post allocate command buffer from pool." << std::endl;
	}

	void Renderer::_renderScene2(const Scene<SpaceType::SPACE_2> *pScene
		, const Camera<SpaceType::SPACE_2> *pCamera
	    , const RenderInfo &info
		, CmdBuffer *pBranchCmdBuffer
        , CmdBuffer *pTrunkRenderPassCmdBuffer
        , CmdBuffer *pTrunkWaitBarrierCmdBuffer
		)
	{
		uint32_t drawCount = 0u;		

		using SceneType = Scene<SpaceType::SPACE_2>;
		auto pDevice = pApp->getDevice();

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        
        fd::CostTimer preparingCommonMatrixsCostTimer(fd::CostTimer::TimerType::ONCE);
		preparingCommonMatrixsCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
		auto projMatrix3x3 = pScene->getProjMatrix(pCamera);
		auto projMatrix = tranMat3ToMat4(projMatrix3x3);
#ifdef USE_WORLD_BOUNDS
		auto boundsOfViewInWorld = pScene->getViewBoundsInWorld(pCamera);
#endif
		
		auto viewMatrix3x3 = pCamera->getTransform()->getMatrixWorldToLocal();
		auto viewMatrix = tranMat3ToMat4(viewMatrix3x3);
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        preparingCommonMatrixsCostTimer.end();
		VG_COST_TIME_LOG(plog::debug) << "Preparing common matrixs cost time: " 
			    << preparingCommonMatrixsCostTimer.costTimer 
				<< "ms, scene id: " << pScene->getID() 
				<< ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
				<<  std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER

		uint32_t visualObjectCount = pScene->getVisualObjectCount();

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        fd::CostTimer visibilityCheckCostTimer(fd::CostTimer::TimerType::ONCE);
		visibilityCheckCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER

		//flat visual objects and filter them that is out of camera with its bounds.
		//allocate enough space for array to storage points.
		std::vector<SceneType::VisualObjectType *> validVisualObjects(visualObjectCount);
		uint32_t validVisualObjectCount(0u);
		auto pRoot = pScene->pRootTransformForVisualObject;
		fillValidVisualObjects(validVisualObjects
			, validVisualObjectCount
			, pRoot.get()
			, pScene
			, pCamera
#ifdef USE_WORLD_BOUNDS
			, &boundsOfViewInWorld
#endif //USE_WORLD_BOUNDS
		);

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
		visibilityCheckCostTimer.end();
		VG_COST_TIME_LOG(plog::debug) << "Visibility check cost time: " 
			    << visibilityCheckCostTimer.costTimer 
				<< "ms, scene id: " << pScene->getID() 
				<< ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
				<<  std::endl;
        
		fd::CostTimer preparingBuildInDataCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
		fd::CostTimer preparingPipelineCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
		fd::CostTimer preparingCommandBufferCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
#endif //DEBUG and VG_ENABLE_COST_TIMER

		//------Doing render.		
		for (uint32_t i = 0u; i < validVisualObjectCount; ++i)
		{
			auto pVisualObject = validVisualObjects[i];

			BaseVisualObject::BindInfo info = {
                m_framebufferWidth,
				m_framebufferHeight,
				&projMatrix,
				&viewMatrix,
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                &preparingBuildInDataCostTimer,
                &preparingPipelineCostTimer,
                &preparingCommandBufferCostTimer,
#endif //DEBUG and VG_ENABLE_COST_TIMER	
				};

			BaseVisualObject::BindResult result;
			result.pTrunkRenderPassCmdBuffer = pTrunkRenderPassCmdBuffer;
			result.pBranchCmdBuffer = pBranchCmdBuffer;
			result.pTrunkWaitBarrierCmdBuffer = pTrunkWaitBarrierCmdBuffer;
			pVisualObject->bindToRender(info, &result);
		}
		
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
		VG_COST_TIME_LOG(plog::debug) << "Pre object recording callback cost time: "
			<< preObjectRecordingCallBackCostTimer.costTimer
			<< "ms, scene id: " << pScene->getID()
			<< ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2")
			<< std::endl;
		VG_COST_TIME_LOG(plog::debug) << "Preparing buildin data cost time: " 
		    << preparingBuildInDataCostTimer.costTimer 
		    << "ms, scene id: " << pScene->getID() 
		    << ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
		    <<  std::endl;
		VG_COST_TIME_LOG(plog::debug) << "Preparing pipeline cost time: " 
			<< preparingPipelineCostTimer.costTimer 
			<< "ms, scene id: " << pScene->getID() 
			<< ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
			<<  std::endl;
		VG_COST_TIME_LOG(plog::debug) << "Preparing command buffer cost time: " 
			<< preparingCommandBufferCostTimer.costTimer 
			<< "ms, scene id: " << pScene->getID() 
			<< ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
			<<  std::endl;
		VG_COST_TIME_LOG(plog::debug) << "Post object recording callback cost time: "
			<< postObjectRecordingCallBackCostTimer.costTimer
			<< "ms, scene id: " << pScene->getID()
			<< ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2")
			<< std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER
	}

	void Renderer::_renderScene3(const Scene<SpaceType::SPACE_3> *pScene
		, const Camera<SpaceType::SPACE_3> *pCamera
	    , const RenderInfo &info
		, CmdBuffer *pBranchCmdBuffer
        , CmdBuffer *pTrunkRenderPassCmdBuffer
        , CmdBuffer *pTrunkWaitBarrierCmdBuffer
		)
	{
		using SceneType = Scene<SpaceType::SPACE_3>;

		uint32_t drawCount = 0u;	

		auto queueTypeCount = static_cast<uint32_t>(RenderQueueType::RANGE_SIZE);
		auto pDevice = pApp->getDevice();

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        
        fd::CostTimer preparingCommonMatrixsCostTimer(fd::CostTimer::TimerType::ONCE);
		preparingCommonMatrixsCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER

		auto projMatrix = pScene->getProjMatrix(pCamera);

#ifdef USE_WORLD_BOUNDS
		auto boundsOfViewInWorld = pScene->getViewBoundsInWorld(pCamera);
#endif

		auto viewMatrix = pCamera->getTransform()->getMatrixWorldToLocal();
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        preparingCommonMatrixsCostTimer.end();
		VG_COST_TIME_LOG(plog::debug) << "Preparing common matrixs cost time: " 
			    << preparingCommonMatrixsCostTimer.costTimer 
				<< "ms, scene id: " << pScene->getID() 
				<< ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
				<<  std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER

		uint32_t visualObjectCount = pScene->getVisualObjectCount();
		
		//----------Preparing render.

		//Filter visualObject is out of camera with its bounds.
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        fd::CostTimer visibilityCheckCostTimer(fd::CostTimer::TimerType::ONCE);
		visibilityCheckCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER

		std::vector<SceneType::VisualObjectType *> validVisualObjects(visualObjectCount); //allocate enough space for array to storage points.
		uint32_t validVisualObjectCount(0u);
		for (uint32_t i = 0; i < visualObjectCount; ++i)
		{
			auto pVisualObject = pScene->getVisualObjectWithIndex(i);
			auto pMesh = pVisualObject->getMesh();
			auto isHasBounds = dynamic_cast<SceneType::VisualObjectType::MeshDimType *>(pMesh)->getIsHasBounds();
			if (isHasBounds == VG_FALSE)
			{
				validVisualObjects[validVisualObjectCount++] = pVisualObject;
			}
			else if (pVisualObject->getIsVisibilityCheck() == VG_FALSE)
			{
				validVisualObjects[validVisualObjectCount++] = pVisualObject;
				pVisualObject->setHasClipRect(VG_FALSE);
			}
			else 
			{
			    auto bounds = dynamic_cast<SceneType::VisualObjectType::MeshDimType *>(pMesh)->getBounds();
			    auto pTransform = pVisualObject->getTransform();
#ifdef USE_WORLD_BOUNDS
				auto boundsInWorld = tranBoundsToNewSpace<Vector3>(bounds, pTransform->getMatrixLocalToWorld(), VG_FALSE);      
#endif //USE_WORLD_BOUNDS
				fd::Rect2D clipRect;
#ifdef USE_WORLD_BOUNDS
				if (boundsOfViewInWorld.isIntersects(boundsInWorld) == FD_TRUE && 
				    pScene->isInView(pCamera, boundsInWorld, &clipRect) == VG_TRUE)
#else 
                if (pScene->isInView(pCamera, pTransform, bounds, &clipRect) == VG_TRUE)
#endif //USE_WORLD_BOUNDS
			    {
			    	validVisualObjects[validVisualObjectCount++] = pVisualObject;
					//Transform range [-1, 1] to range [0, 1]
				    clipRect.x = (clipRect.x + 1.0f) / 2.0f;
				    clipRect.y = (clipRect.y + 1.0f) / 2.0f;
				    clipRect.width = clipRect.width / 2.0f;
				    clipRect.height = clipRect.height / 2.0f;
    
				    uint32_t subMeshCount = pVisualObject->getSubMeshCount();
				    pVisualObject->setHasClipRect(VG_TRUE);
				    pVisualObject->updateClipRects(clipRect, subMeshCount);
			    }
				
			}
		}

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
		visibilityCheckCostTimer.end();
		VG_COST_TIME_LOG(plog::debug) << "Visibility check cost time: " 
			    << visibilityCheckCostTimer.costTimer 
				<< "ms, scene id: " << pScene->getID() 
				<< ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
				<<  std::endl;
        
		fd::CostTimer preparingBuildInDataCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
		fd::CostTimer preparingPipelineCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
		fd::CostTimer preparingCommandBufferCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
		fd::CostTimer preObjectRecordingCallBackCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
		fd::CostTimer postObjectRecordingCallBackCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
#endif //DEBUG and VG_ENABLE_COST_TIMER

		//Get queue count for each queue type.
		std::vector<uint32_t> queueLengths(queueTypeCount, 0u);
		for (uint32_t i = 0; i < validVisualObjectCount; ++i)
		{
			auto pVisualObject = validVisualObjects[i];
			auto renderQueueType = tranMaterialShowTypeToRenderQueueType(pVisualObject->getMaterial()->getShowType());
			++queueLengths[static_cast<size_t>(renderQueueType)];
		}

		std::vector<std::vector<SceneType::VisualObjectType *>> queues(queueTypeCount);
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
			[&viewMatrix, &projMatrix](typename SceneType::ObjectType *pObject1, typename SceneType::ObjectType *pObject2)
	        {
				auto modelMatrix1 = pObject1->getTransform()->getMatrixLocalToWorld();
				auto mvMatrix1 = viewMatrix * modelMatrix1;
				auto modelMatrix2 = pObject2->getTransform()->getMatrixLocalToWorld();
				auto mvMatrix2 = viewMatrix * modelMatrix2;
	        	//get position
	        	auto pos1 = pObject1->getTransform()->getLocalPosition();
	        	auto pos2 = pObject2->getTransform()->getLocalPosition();
        
	        	using PointType = SpaceTypeInfo<SpaceType::SPACE_3>::PointType;
	        	using MatrixVectorType = SpaceTypeInfo<SpaceType::SPACE_3>::MatrixVectorType;
	        	//transform point from model coordinate system to camera coordinate system.
	        	pos1 = mvMatrix1 * MatrixVectorType(pos1, 1.0f);
	        	pos2 = mvMatrix2 * MatrixVectorType(pos2, 1.0f);
        
	        	//it is smaller if its z is bigger than the other.
	        	return static_cast<Bool32>(pos1.z > pos2.z);
        
	        });

		//-----Doing render
		for (uint32_t typeIndex = 0u; typeIndex < queueTypeCount; ++typeIndex)
		{
			auto queueLength = queueLengths[typeIndex];
			for (uint32_t objectIndex = 0u; objectIndex < queueLength; ++objectIndex)
			{
				auto pVisualObject = queues[typeIndex][objectIndex];

				BaseVisualObject::BindInfo info = {
                    m_framebufferWidth,
				    m_framebufferHeight,
				    &projMatrix,
				    &viewMatrix,
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                    &preparingBuildInDataCostTimer,
                    &preparingPipelineCostTimer,
                    &preparingCommandBufferCostTimer,
#endif //DEBUG and VG_ENABLE_COST_TIMER	
				    };

			    BaseVisualObject::BindResult result;
				result.pTrunkRenderPassCmdBuffer = pTrunkRenderPassCmdBuffer;
			    result.pBranchCmdBuffer = pBranchCmdBuffer;
				result.pTrunkWaitBarrierCmdBuffer = pTrunkWaitBarrierCmdBuffer;
			    pVisualObject->bindToRender(info, &result);
			}
		}

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
		VG_COST_TIME_LOG(plog::debug) << "Pre object recording callback cost time: "
			<< preObjectRecordingCallBackCostTimer.costTimer
			<< "ms, scene id: " << pScene->getID()
			<< ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2")
			<< std::endl;
		VG_COST_TIME_LOG(plog::debug) << "Preparing buildin data cost time: " 
		    << preparingBuildInDataCostTimer.costTimer 
		    << "ms, scene id: " << pScene->getID() 
		    << ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
		    <<  std::endl;
		VG_COST_TIME_LOG(plog::debug) << "Preparing pipeline cost time: " 
			<< preparingPipelineCostTimer.costTimer 
			<< "ms, scene id: " << pScene->getID() 
			<< ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
			<<  std::endl;
		VG_COST_TIME_LOG(plog::debug) << "Preparing command buffer cost time: " 
			<< preparingCommandBufferCostTimer.costTimer 
			<< "ms, scene id: " << pScene->getID() 
			<< ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
			<<  std::endl;
		VG_COST_TIME_LOG(plog::debug) << "Post object recording callback cost time: "
			<< postObjectRecordingCallBackCostTimer.costTimer
			<< "ms, scene id: " << pScene->getID()
			<< ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2")
			<< std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER
	}

	void fillValidVisualObjects(std::vector<VisualObject<SpaceType::SPACE_2> *> &arrPVObjs
	    , uint32_t &PVObjIndex
		, const Transform<SpaceType::SPACE_2> *pTransform
		, const Scene<SpaceType::SPACE_2> *pScene
		, const Camera<SpaceType::SPACE_2> *pCamera
#ifdef USE_WORLD_BOUNDS
		, const fd::Bounds<SpaceTypeInfo<SpaceType::SPACE_2>::PointType> *pViewBoundsInWorld
#endif
		)
	{
		VisualObject<SpaceType::SPACE_2> *pVisualObjectOfChild;
		uint32_t childCount = pTransform->getChildCount();
		Transform<SpaceType::SPACE_2> *pChild;
		for (uint32_t i = 0; i < childCount; ++i)
		{
			pChild = pTransform->getChildWithIndex(i);
			//Children's visual object is placed ahead of own visual objects
			fillValidVisualObjects(arrPVObjs
			    , PVObjIndex
				, pChild
				, pScene
				, pCamera
#ifdef USE_WORLD_BOUNDS
				, pViewBoundsInWorld
#endif //USE_WORLD_BOUNDS
				);
			//Own visual object is placed behind children's visual object.
			pVisualObjectOfChild = pScene->getVisualObjectWithTransform(pChild);
			if (pVisualObjectOfChild == nullptr) continue;
			
			auto pMeshOfChild = pVisualObjectOfChild->getMesh();
			auto isHasBoundsOfChild = dynamic_cast<Mesh<MeshDimType::SPACE_2> *>(pMeshOfChild)->getIsHasBounds();
			if (isHasBoundsOfChild == VG_FALSE)
			{
				arrPVObjs[PVObjIndex++] = pVisualObjectOfChild;
			} 
			else if (pVisualObjectOfChild->getIsVisibilityCheck() == VG_FALSE)
			{
				arrPVObjs[PVObjIndex++] = pVisualObjectOfChild;
				pVisualObjectOfChild->setHasClipRect(VG_FALSE);
			}
			else {
				//Filter obj out of camera view.
			    auto boundsOfChild = dynamic_cast<Mesh<MeshDimType::SPACE_2> *>(pMeshOfChild)->getBounds();
#ifdef USE_WORLD_BOUNDS
				auto boundsOfChildInWorld = tranBoundsToNewSpace<Vector2>(boundsOfChild, pChild->getMatrixLocalToWorld(), VG_FALSE);
#endif //USE_WORLD_BOUNDS
			    fd::Rect2D clipRect;	
#ifdef USE_WORLD_BOUNDS
				if (pViewBoundsInWorld->isIntersects(boundsOfChildInWorld) && pScene->isInView(pCamera, boundsOfChildInWorld, &clipRect) == VG_TRUE)
#else 
                if (pScene->isInView(pCamera, pChild, boundsOfChild, &clipRect) == VG_TRUE)
#endif //USE_WORLD_BOUNDS
			    {
			    	arrPVObjs[PVObjIndex++] = pVisualObjectOfChild;
					//Transform range [-1, 1] to range [0, 1]
				    clipRect.x = (clipRect.x + 1.0f) / 2.0f;
				    clipRect.y = (clipRect.y + 1.0f) / 2.0f;
				    clipRect.width = clipRect.width / 2.0f;
				    clipRect.height = clipRect.height / 2.0f;
				    uint32_t subMeshCount = pVisualObjectOfChild->getSubMeshCount();
				    pVisualObjectOfChild->setHasClipRect(VG_TRUE);
				    pVisualObjectOfChild->updateClipRects(clipRect, subMeshCount);
			    }
			}
		}
	}
}