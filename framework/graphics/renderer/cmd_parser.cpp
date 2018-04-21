#include "graphics/renderer/cmd_parser.hpp"

namespace vg
{
    CMDParser::ResultInfo::ResultInfo(uint32_t drawCount)
		: drawCount(drawCount)
    {
    }
	
    void CMDParser::recordTrunk(CmdBuffer *pCmdBuffer
        ,  vk::CommandBuffer *pCommandBuffer
        , PipelineCache *pPipelineCache
		, vk::RenderPass *pRenderPass
        , ResultInfo *pResult
        )
    {
		auto cmdInfoCount = pCmdBuffer->getCmdCount();
		for (uint32_t cmdInfoIndex = 0u; cmdInfoIndex < cmdInfoCount; ++cmdInfoIndex)
		{
			auto pRenderPassInfo = (pCmdBuffer->getCmdInfos() + cmdInfoIndex)->pRenderPassInfo;
			if (pRenderPassInfo == nullptr) throw std::runtime_error("There isn't render pass info in trunk render pass cmd buffer.");
			auto tempRenderPassInfo = *pRenderPassInfo;
			tempRenderPassInfo.pRenderPass = pRenderPass;
			recordItem(&tempRenderPassInfo, pCommandBuffer, pPipelineCache, pResult);
		}
	}

	void CMDParser::recordTrunkWaitBarrier(CmdBuffer *pTrunkWaitBarrierCmdBuffer
            , vk::CommandBuffer *pCommandBuffer
            )
	{
		if (pTrunkWaitBarrierCmdBuffer->getCmdCount() == 0) return;
		vk::PipelineStageFlags srcStageMask = vk::PipelineStageFlags();
		vk::PipelineStageFlags dstStageMask = vk::PipelineStageFlags();
		vk::DependencyFlags dependencyFlags = vk::DependencyFlags();
		uint32_t cmdCount = pTrunkWaitBarrierCmdBuffer->getCmdCount();
		for (uint32_t i = 0; i < cmdCount; ++i)
		{
			const auto &cmdInfo = *(pTrunkWaitBarrierCmdBuffer->getCmdInfos() + i);
			if (cmdInfo.pRenderPassInfo != nullptr) 
			    VG_LOG(plog::warning) << "The render pass info in trunk wait barrier cmd buffer is invalid." << std::endl;
			const auto &trunkWaitBarrierInfo = *(cmdInfo.pBarrierInfo);
			// if (trunkWaitBarrierInfo.memoryBarrierCount > 0) 
			//     VG_LOG(plog::warning) << "The memory barrier in trunk wait barrier cmd buffer is invalid." << std::endl;
			// if (trunkWaitBarrierInfo.bufferMemoryBarrierCount > 0) 
			//     VG_LOG(plog::warning) << "The buffer memory barrier in trunk wait barrier cmd buffer is invalid." << std::endl;
			// if (trunkWaitBarrierInfo.imageMemoryBarrierCount > 0) 
			//     VG_LOG(plog::warning) << "The image memory barrier in trunk wait barrier cmd buffer is invalid." << std::endl;
			// srcStageMask |= trunkWaitBarrierInfo.srcStageMask;
			// dstStageMask |= trunkWaitBarrierInfo.dstStageMask;
			// dependencyFlags |= trunkWaitBarrierInfo.dependencyFlags;
			pCommandBuffer->pipelineBarrier(trunkWaitBarrierInfo.srcStageMask, 
		        trunkWaitBarrierInfo.dstStageMask, 
			    trunkWaitBarrierInfo.dependencyFlags,
			    trunkWaitBarrierInfo.memoryBarrierCount,
				trunkWaitBarrierInfo.pMemoryBarriers,
			    trunkWaitBarrierInfo.bufferMemoryBarrierCount,
				trunkWaitBarrierInfo.pBufferMemoryBarriers,
				trunkWaitBarrierInfo.imageMemoryBarrierCount,
			    trunkWaitBarrierInfo.pImageMemoryBarriers
				);
		}

		// pCommandBuffer->pipelineBarrier(srcStageMask, 
		//     dstStageMask, 
		// 	dependencyFlags,
		// 	nullptr,
		// 	nullptr,
		// 	nullptr);
	}

	void CMDParser::recordBranch(CmdBuffer *pCmdBuffer
        ,  vk::CommandBuffer *pCommandBuffer
        , PipelineCache *pPipelineCache
        , ResultInfo *pResult
        )
    {
		uint32_t drawCount = 0u;
		auto cmdInfoCount = pCmdBuffer->getCmdCount();
		vk::RenderPass lastRenderPass = vk::RenderPass();
        uint32_t lastSubPassIndex = 0u;
		for (uint32_t cmdInfoIndex = 0u; cmdInfoIndex < cmdInfoCount; ++cmdInfoIndex)
		{
			auto pRenderPassInfo = (pCmdBuffer->getCmdInfos() + cmdInfoIndex)->pRenderPassInfo;
			if (pRenderPassInfo)
			{
				if (lastRenderPass == vk::RenderPass() || lastRenderPass != *(pRenderPassInfo->pRenderPass)) {
					if (pRenderPassInfo->subPassIndex != 0) {
						throw std::runtime_error("Error of start of subpass index of render pass for cmd info.");
					}
					recordItemRenderPassBegin(pRenderPassInfo, pCommandBuffer);
				} else if (pRenderPassInfo->subPassIndex - lastSubPassIndex == 1u) {
					recordItemNextSubpass(pCommandBuffer);
				} else if (pRenderPassInfo->subPassIndex - lastSubPassIndex != 0u) {
					throw std::runtime_error("Error of increasing of subpass index of render pass for cmd info.");
				} //else it is inner sub pass.
				
			    recordItem(pRenderPassInfo, pCommandBuffer, pPipelineCache, pResult);

				//do end render pass.
				const RenderPassInfo * pNextRenderPassInfo = nullptr;
				uint32_t nextIndex = cmdInfoIndex + 1;
				while (nextIndex < cmdInfoCount && pNextRenderPassInfo == nullptr) {
					pNextRenderPassInfo = (pCmdBuffer->getCmdInfos() + nextIndex)->pRenderPassInfo;
					++nextIndex;
				}
				if (pNextRenderPassInfo == nullptr || *(pNextRenderPassInfo->pRenderPass) != *(pRenderPassInfo->pRenderPass)) {
					recordItemRenderPassEnd(pRenderPassInfo, pCommandBuffer);
				}

				lastRenderPass = *(pRenderPassInfo->pRenderPass);
				lastSubPassIndex = pRenderPassInfo->subPassIndex;
				++drawCount;
			}
			else 
		    {
				auto pBarrierInfo = (pCmdBuffer->getCmdInfos() + cmdInfoIndex)->pBarrierInfo;
			    if (pBarrierInfo != nullptr)
			    {
					pCommandBuffer->pipelineBarrier(pBarrierInfo->srcStageMask
					    , pBarrierInfo->dstStageMask
						, pBarrierInfo->dependencyFlags
						, pBarrierInfo->memoryBarrierCount
						, pBarrierInfo->pMemoryBarriers
						, pBarrierInfo->bufferMemoryBarrierCount
						, pBarrierInfo->pBufferMemoryBarriers
						, pBarrierInfo->imageMemoryBarrierCount
						, pBarrierInfo->pImageMemoryBarriers
						);
			    }
			}
			
		}

		if (pResult != nullptr)pResult->drawCount = drawCount;
	}

	void CMDParser::recordItemRenderPassBegin(const RenderPassInfo *pRenderPassInfo
            ,  vk::CommandBuffer *pCommandBuffer)
	{
		uint32_t framebufferWidth = pRenderPassInfo->framebufferWidth;
		uint32_t framebufferHeight = pRenderPassInfo->framebufferHeight;
		auto renderArea = pRenderPassInfo->renderArea;
		vk::RenderPassBeginInfo renderPassBeginInfo = {
			* (pRenderPassInfo->pRenderPass), 
			* (pRenderPassInfo->pFrameBuffer),
			vk::Rect2D(           
				vk::Offset2D(static_cast<int32_t>(std::round(framebufferWidth * renderArea.x))
					, static_cast<int32_t>(std::round(framebufferHeight * renderArea.y))
				),
				vk::Extent2D(static_cast<uint32_t>(std::round(framebufferWidth * renderArea.width)),
					static_cast<uint32_t>(std::round(framebufferHeight * renderArea.height))
				)
			),
			pRenderPassInfo->clearValueCount,
			pRenderPassInfo->pClearValues
		};

		pCommandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
	}

    void CMDParser::recordItemRenderPassEnd(const RenderPassInfo *pRenderPassInfo
            ,  vk::CommandBuffer *pCommandBuffer)
	{
		pCommandBuffer->endRenderPass();
	}

	void CMDParser::recordItemRenderPassEnd(vk::CommandBuffer *pCommandBuffer)
	{
		pCommandBuffer->endRenderPass();
	}

	void CMDParser::recordItemNextSubpass(vk::CommandBuffer *pCommandBuffer)
	{
		pCommandBuffer->nextSubpass(vk::SubpassContents::eInline);
	}

	void CMDParser::recordItem(const RenderPassInfo *pRenderPassInfo
        ,  vk::CommandBuffer *pCommandBuffer
        , PipelineCache *pPipelineCache
        , ResultInfo *pResult)
	{

		const auto &renderPassInfo = *pRenderPassInfo; 
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
		renderPassInfo.pPreparingBuildInDataCostTimer->begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
		Bool32 hasMatrixObjectToNDC = VG_FALSE;
		Bool32 hasMatrixObjectToWorld = VG_FALSE;
		Bool32 hasMatrixObjectToView = VG_FALSE;
		Bool32 hasMatrixView = VG_FALSE;
		Bool32 hasMatrixProjection = VG_FALSE;
		//update building in matrix variable.
		auto pPass = renderPassInfo.pPass;
		auto info = pPass->getBuildInDataInfo();
		uint32_t componentCount = info.componentCount;
		for (uint32_t componentIndex = 0u; componentIndex < componentCount; ++componentIndex)
		{
			Pass::BuildInDataType type = (*(info.pComponent + componentIndex)).type;
			if (type == Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC)
			{
				hasMatrixObjectToNDC = VG_TRUE;
			}
			else if (type == Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD)
			{
				hasMatrixObjectToWorld = VG_TRUE;
			}
			else if (type == Pass::BuildInDataType::MATRIX_OBJECT_TO_VIEW)
			{
				hasMatrixObjectToView = VG_TRUE;
			}
			else if (type == Pass::BuildInDataType::MATRIX_VIEW)
			{
				hasMatrixView = VG_TRUE;
			}
			else if (type == Pass::BuildInDataType::MATRIX_PROJECTION)
			{
				hasMatrixProjection = VG_TRUE;
			}
		}
		
		Matrix4x4 mvMatrix;
		Matrix4x4 mvpMatrix;
		if (hasMatrixObjectToView || hasMatrixObjectToNDC)
		{
			mvMatrix = renderPassInfo.viewMatrix * renderPassInfo.modelMatrix;
        }
		
		if (hasMatrixObjectToNDC)
		{
			mvpMatrix = renderPassInfo.projMatrix * mvMatrix;
		}
		auto pMesh = renderPassInfo.pMesh;
		auto pContentMesh = dynamic_cast<const ContentMesh *>(pMesh);
		auto subMeshIndex = renderPassInfo.subMeshIndex;
		//update building in matrix variable.
		for (uint32_t componentIndex = 0u; componentIndex < componentCount; ++componentIndex)
		{
			Pass::BuildInDataType type = (*(info.pComponent + componentIndex)).type;
			if (type == Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC)
			{
				pPass->_setBuildInMatrixData(type, mvpMatrix);
			}
			else if (type == Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD)
			{
				pPass->_setBuildInMatrixData(type, renderPassInfo.modelMatrix);
			}
			else if (type == Pass::BuildInDataType::MATRIX_OBJECT_TO_VIEW)
			{
				pPass->_setBuildInMatrixData(type, mvMatrix);
			}
			else if (type == Pass::BuildInDataType::MATRIX_VIEW)
			{
				pPass->_setBuildInMatrixData(type, renderPassInfo.viewMatrix);
			}
			else if (type == Pass::BuildInDataType::MATRIX_PROJECTION)
			{
				pPass->_setBuildInMatrixData(type, renderPassInfo.projMatrix);
			}
		}
		pPass->apply();
		

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
		renderPassInfo.pPreparingBuildInDataCostTimer->end();
#endif //DEBUG and VG_ENABLE_COST_TIMER
		auto pShader = pPass->getShader();
		auto stageInfos = pShader->getShaderStageInfos();
		if (stageInfos.size() != 0)
		{
			std::shared_ptr<vk::Pipeline> pPipeline;
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
			renderPassInfo.pPreparingPipelineCostTimer->begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
			_createPipeline(renderPassInfo.pRenderPass,
				renderPassInfo.pMesh,
				subMeshIndex, 
				pPass, 
				pPipelineCache,
				pPipeline);
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
			renderPassInfo.pPreparingPipelineCostTimer->end();
			renderPassInfo.pPreparingCommandBufferCostTimer->begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
			_recordCommandBuffer(pPipeline.get(),
				pCommandBuffer,
				renderPassInfo.framebufferWidth,
				renderPassInfo.framebufferHeight,
				renderPassInfo.pMesh,
				subMeshIndex, 
				pPass, 
				renderPassInfo.viewport,
				renderPassInfo.scissor,
				renderPassInfo.pCmdDraw,
				renderPassInfo.pCmdDrawIndexed
			);
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
			renderPassInfo.pPreparingCommandBufferCostTimer->end();
#endif //DEBUG and VG_ENABLE_COST_TIMER
		}
		else
		{
			VG_LOG(plog::warning) << "No one valid shader module for pass. Pass ID: " << pPass->getID() << std::endl;
		}
    }

    void CMDParser::_createPipeline(vk::RenderPass *pRenderPass,
		const BaseMesh *pMesh,
		uint32_t subMeshIndex,
		Pass *pPass,
		PipelineCache *pPipelineCache,
		std::shared_ptr<vk::Pipeline> &pPipeline)
	{
		const vg::ContentMesh *pContentMesh = nullptr;
		if (pMesh != nullptr) {
			pContentMesh = dynamic_cast<const ContentMesh *>(pMesh);
		} 
		PipelineCache::Info info(
			*pRenderPass,
			pPass,
			pContentMesh != nullptr ? pContentMesh->getVertexData() : nullptr,
			0u,
			pContentMesh != nullptr ? pContentMesh->getIndexData() : nullptr,
			subMeshIndex
		);
		pPipeline = pPipelineCache->caching(info);
	}

	void CMDParser::_recordCommandBuffer(vk::Pipeline *pPipeline,
	    vk::CommandBuffer *pCommandBuffer,
		uint32_t framebufferWidth,
		uint32_t framebufferHeight,
		const BaseMesh *pMesh,
		uint32_t subMeshIndex,
		Pass *pPass,
		const fd::Viewport viewport,
		const fd::Rect2D scissor,
		CmdDraw * pCmdDraw,
        CmdDrawIndexed * pCmdDrawIndexed
		)
	{        
        const auto& viewportOfPass = pPass->getViewport();
		const auto& scissorOfPass = pPass->getScissor();

		fd::Viewport finalViewport;
		//The mesh viewport is base on the viewport of pass.
		finalViewport.x = viewport.x * viewportOfPass.width + viewportOfPass.x;
		finalViewport.y = viewport.y * viewportOfPass.height + viewportOfPass.y;
		finalViewport.width = viewport.width * viewportOfPass.width;
		finalViewport.height = viewport.height * viewportOfPass.height;
		finalViewport.minDepth = viewport.minDepth * viewportOfPass.minDepth;
		finalViewport.maxDepth = viewport.maxDepth * viewportOfPass.maxDepth;

		//View port info.
		vk::Viewport vkViewport = {
			(float)framebufferWidth * finalViewport.x,                      //x
			(float)framebufferHeight * finalViewport.y,                     //y
			(float)framebufferWidth * finalViewport.width,                  //width
			(float)framebufferHeight * finalViewport.height,                 //height
			1.0f * finalViewport.minDepth,                                     //minDepth
			1.0f * finalViewport.maxDepth                                      //maxDepth
		};

		pCommandBuffer->setViewport(0, vkViewport);

		fd::Rect2D finalScissor = scissorOfPass;
		glm::vec2 minOfClipRect(scissor.x, scissor.y);
		glm::vec2 maxOfclipRect(scissor.x + scissor.width, scissor.y + scissor.height);
		fd::Bounds<glm::vec2> boundsOfClipRect(minOfClipRect, maxOfclipRect);
		glm::vec2 minOfScissorOfPass(scissorOfPass.x, scissorOfPass.y);
		glm::vec2 maxOfScissorOfPass(scissorOfPass.x + scissorOfPass.width, scissorOfPass.y + scissorOfPass.height);
		fd::Bounds<glm::vec2> boundsOfScissorOfPass(minOfScissorOfPass, maxOfScissorOfPass);
		fd::Bounds<glm::vec2> intersection;
		if (boundsOfScissorOfPass.intersects(boundsOfClipRect, &intersection))
		{
			auto min = intersection.getMin();
			auto size = intersection.getSize();
			finalScissor.x = min.x;
			finalScissor.y = min.y;
			finalScissor.width = size.x;
			finalScissor.height = size.y;
		}

		vk::Rect2D vkScissor = {
			{                               //offset
				static_cast<int32_t>(std::floorf((float)framebufferWidth * viewportOfPass.x + 
				    (float)framebufferWidth * viewportOfPass.width * finalScissor.x)),    //x
				static_cast<int32_t>(std::floorf((float)framebufferHeight * viewportOfPass.y +
			        (float)framebufferHeight * viewportOfPass.height * finalScissor.y))    //y
			},
			{                               //extent
				static_cast<uint32_t>(std::ceilf((float)framebufferWidth * viewportOfPass.width * finalScissor.width)),   //width
				static_cast<uint32_t>(std::ceilf((float)framebufferHeight * viewportOfPass.height * finalScissor.height))  //height
			}
		};

		pCommandBuffer->setScissor(0, vkScissor);

		auto pPipelineLayout = pPass->getPipelineLayout();	

		//push constants
		auto pushConstantUpdates = pPass->getPushconstantUpdates();
		for(const auto& pPushConstantUpdate : pushConstantUpdates)
		{
			pCommandBuffer->pushConstants(*pPipelineLayout, 
			    pPushConstantUpdate->getStageFlags(), 
				pPushConstantUpdate->getOffset(),
				pPushConstantUpdate->getSize(),
				pPushConstantUpdate->getData());
		}


		pCommandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, *pPipeline);

		uint32_t descriptSetCount = pPass->getUsingDescriptorSetCount();
		auto pDescriptorSets = pPass->getUsingDescriptorSets();
		uint32_t dynamicOffsetCount = pPass->getUsingDescriptorDynamicOffsetCount();
		auto pDynamicOffsets = pPass->getUsingDescriptorDynamicOffsets();

		pCommandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, *pPipelineLayout, 
		    0u, descriptSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);

		//dynamic line width
		pCommandBuffer->setLineWidth(pPass->getLineWidth());

		if (pMesh != nullptr) {
		    auto pContentMesh = dynamic_cast<const ContentMesh *>(pMesh);
		    const auto &pVertexData = pContentMesh->getVertexData();
		    const auto &subVertexDatas = pVertexData->getSubVertexDatas();
		    const auto &pIndexData = pContentMesh->getIndexData();
		    const auto &subIndexDatas = pIndexData->getSubIndexDatas();
		    const auto &subIndexData = subIndexDatas[subMeshIndex];
    
            vertexDataToCommandBuffer(*pCommandBuffer, pVertexData, subIndexData.vertexDataIndex);
		    indexDataToCommandBuffer(*pCommandBuffer, pIndexData, subMeshIndex);
		}

		if (pCmdDraw != nullptr) {
			pCommandBuffer->draw(pCmdDraw->vertexCount, 
			    pCmdDraw->instanceCount,
				pCmdDraw->firstVertex,
				pCmdDraw->firstInstance
				);
		} else if (pCmdDrawIndexed != nullptr) {
			pCommandBuffer->drawIndexed(pCmdDrawIndexed->indexCount,
			    pCmdDrawIndexed->instanceCount,
				pCmdDrawIndexed->firstIndex,
				pCmdDrawIndexed->vertexOffset,
				pCmdDrawIndexed->firstInstance
				);
		} else if (pMesh != nullptr) {
			auto pContentMesh = dynamic_cast<const ContentMesh *>(pMesh);
			const auto &pIndexData = pContentMesh->getIndexData();
		    const auto &subIndexDatas = pIndexData->getSubIndexDatas();
		    const auto &subIndexData = subIndexDatas[subMeshIndex];

		    uint32_t indexOffset = 0u;
		    // for (uint32_t i = 0; i < subMeshIndex; ++i)
		    // {
		    // 	indexOffset += subIndexDatas[i].indexCount;
		    // }
    
		    uint32_t vertexOffset = 0u;
		    // for (uint32_t i = 0; i < subIndexData.vertexDataIndex; ++i)
		    // {
		    // 	vertexOffset += subVertexDatas[i].vertexCount;
		    // }
    
		    uint32_t instanceOffset = 0u;
		    uint32_t instanceCount = pPass->getInstanceCount();
    
		    pCommandBuffer->drawIndexed(subIndexData.indexCount, 
		        instanceCount, 
		    	indexOffset, 
		    	vertexOffset, 
		    	instanceOffset);
		} else {

		}
		//m_pCommandBuffer->draw(3, 1, 0, 0);
	}

} //vg