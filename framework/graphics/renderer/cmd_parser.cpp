#include "graphics/renderer/cmd_parser.hpp"

namespace vg
{
    CMDParser::ResultInfo::ResultInfo()
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
			pRenderPassInfo->pRenderPass = pRenderPass;
			recordItem(pRenderPassInfo, pCommandBuffer, pPipelineCache, pResult);
		}
	}

	void CMDParser::recordBranch(CmdBuffer *pCmdBuffer
        ,  vk::CommandBuffer *pCommandBuffer
        , PipelineCache *pPipelineCache
        , ResultInfo *pResult
        )
    {
		auto cmdInfoCount = pCmdBuffer->getCmdCount();
		for (uint32_t cmdInfoIndex = 0u; cmdInfoIndex < cmdInfoCount; ++cmdInfoIndex)
		{
			auto pRenderPassInfo = (pCmdBuffer->getCmdInfos() + cmdInfoIndex)->pRenderPassInfo;
			if (pRenderPassInfo)
			{
			    recordItemRenderPassBegin(pRenderPassInfo, pCommandBuffer);
			    recordItem(pRenderPassInfo, pCommandBuffer, pPipelineCache, pResult);
			    recordItemRenderPassEnd(pRenderPassInfo, pCommandBuffer);
			}
			else 
		    {
				auto barrierInfo = (pCmdBuffer->getCmdInfos() + cmdInfoIndex)->pBarrierInfo;
			    if (barrierInfo)
			    {
    
			    }
			}
			
		}
	}

	void CMDParser::recordItemRenderPassBegin(RenderPassInfo *pRenderPassInfo
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

    void CMDParser::recordItemRenderPassEnd(RenderPassInfo *pRenderPassInfo
            ,  vk::CommandBuffer *pCommandBuffer)
	{
		pCommandBuffer->endRenderPass();
	}

	void CMDParser::recordItem(RenderPassInfo *pRenderPassInfo
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
            const fd::Rect2D *pClipRect;
			if(renderPassInfo.hasClipRect)
			{
				pClipRect = renderPassInfo.pClipRect;
			}
			else
			{
				pClipRect = nullptr;
			}
			_recordCommandBuffer(pPipeline.get(),
				pCommandBuffer,
				renderPassInfo.framebufferWidth,
				renderPassInfo.framebufferHeight,
				renderPassInfo.pMesh,
				subMeshIndex, 
				pPass, 
				pClipRect);
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
		auto pContentMesh = dynamic_cast<const ContentMesh *>(pMesh);
		PipelineCache::Info info(
			*pRenderPass,
			pPass,
			pContentMesh->getVertexData(),
			0u,
			pContentMesh->getIndexData(),
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
		const fd::Rect2D *pClipRect)
	{
		VG_LOG(plog::debug) << "Pre begin command buffer for render." << std::endl;
		auto pContentMesh = dynamic_cast<const ContentMesh *>(pMesh);
        
        const auto& viewportOfPass = pPass->getViewport();
		const auto& scissorOfPass = pPass->getScissor();
		//View port info.
		vk::Viewport viewport = {
			(float)framebufferWidth * viewportOfPass.x,                      //x
			(float)framebufferHeight * viewportOfPass.y,                     //y
			(float)framebufferWidth * viewportOfPass.width,                  //width
			(float)framebufferHeight * viewportOfPass.height,                 //height
			1.0f * viewportOfPass.minDepth,                                     //minDepth
			1.0f * viewportOfPass.maxDepth                                      //maxDepth
		};

		pCommandBuffer->setViewport(0, viewport);

		const auto &pVertexData = pContentMesh->getVertexData();
		const auto &subVertexDatas = pVertexData->getSubVertexDatas();
		const auto &pIndexData = pContentMesh->getIndexData();
		const auto &subIndexDatas = pIndexData->getSubIndexDatas();
		const auto &subIndexData = subIndexDatas[subMeshIndex];
		fd::Rect2D finalScissor(0.0f, 0.0f, 0.0f, 0.0f);
		if (pClipRect != nullptr)
		{

			auto clipRect = *pClipRect;
			glm::vec2 minOfClipRect(clipRect.x, clipRect.y);
			glm::vec2 maxOfclipRect(clipRect.x + clipRect.width, clipRect.y + clipRect.height);

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
		}
		else
		{
			finalScissor = scissorOfPass;
		}

		vk::Rect2D scissor = {
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

		pCommandBuffer->setScissor(0, scissor);

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

        vertexDataToCommandBuffer(*pCommandBuffer, pVertexData, subIndexData.vertexDataIndex);
		indexDataToCommandBuffer(*pCommandBuffer, pIndexData, subMeshIndex);

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
		//m_pCommandBuffer->draw(3, 1, 0, 0);
	}

} //vg