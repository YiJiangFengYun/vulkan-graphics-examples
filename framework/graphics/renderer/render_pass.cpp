#include "graphics/renderer/render_pass.hpp"

namespace vg
{
    RenderPass::ResultInfo::ResultInfo()
    {
    }
    void RenderPass::record(uint32_t itemCount
        , RenderPassInfo *pItems
        ,  vk::CommandBuffer *pCommandBuffer
        , PipelineCache *pPipelineCache
        , ResultInfo *pResult
        )
    {
		for (uint32_t itemIndex = 0u; itemIndex < itemCount; ++itemIndex)
		{
			recordItem(pItems + itemIndex, pCommandBuffer, pPipelineCache, pResult);
		}
	}

	void RenderPass::recordItem(RenderPassInfo *pItem
        ,  vk::CommandBuffer *pCommandBuffer
        , PipelineCache *pPipelineCache
        , ResultInfo *pResult)
	{
		const auto &itemInfo = *pItem; 
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
		itemInfo.pPreparingBuildInDataCostTimer->begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
		Bool32 hasMatrixObjectToNDC = VG_FALSE;
		Bool32 hasMatrixObjectToWorld = VG_FALSE;
		Bool32 hasMatrixObjectToView = VG_FALSE;
		Bool32 hasMatrixView = VG_FALSE;
		Bool32 hasMatrixProjection = VG_FALSE;
		//update building in matrix variable.
		auto pPass = itemInfo.pPass;
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
			mvMatrix = itemInfo.viewMatrix * itemInfo.modelMatrix;
        }
		
		if (hasMatrixObjectToNDC)
		{
			mvpMatrix = itemInfo.projMatrix * mvMatrix;
		}
		auto pMesh = itemInfo.pMesh;
		auto pContentMesh = dynamic_cast<const ContentMesh *>(pMesh);
		auto subMeshIndex = itemInfo.subMeshIndex;
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
				pPass->_setBuildInMatrixData(type, itemInfo.modelMatrix);
			}
			else if (type == Pass::BuildInDataType::MATRIX_OBJECT_TO_VIEW)
			{
				pPass->_setBuildInMatrixData(type, mvMatrix);
			}
			else if (type == Pass::BuildInDataType::MATRIX_VIEW)
			{
				pPass->_setBuildInMatrixData(type, itemInfo.viewMatrix);
			}
			else if (type == Pass::BuildInDataType::MATRIX_PROJECTION)
			{
				pPass->_setBuildInMatrixData(type, itemInfo.projMatrix);
			}
		}
		pPass->apply();
		

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
		itemInfo.pPreparingBuildInDataCostTimer->end();
#endif //DEBUG and VG_ENABLE_COST_TIMER
		auto pShader = pPass->getShader();
		auto stageInfos = pShader->getShaderStageInfos();
		if (stageInfos.size() != 0)
		{
			std::shared_ptr<vk::Pipeline> pPipeline;
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
			itemInfo.pPreparingPipelineCostTimer->begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
			_createPipeline(itemInfo.pRenderPass,
				itemInfo.pMesh,
				subMeshIndex, 
				pPass, 
				pPipelineCache,
				pPipeline);
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
			itemInfo.pPreparingPipelineCostTimer->end();
			itemInfo.pPreparingCommandBufferCostTimer->begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
            const fd::Rect2D *pClipRect;
			if(itemInfo.hasClipRect)
			{
				pClipRect = itemInfo.pClipRect;
			}
			else
			{
				pClipRect = nullptr;
			}
			_recordCommandBuffer(pPipeline.get(),
				pCommandBuffer,
				itemInfo.framebufferWidth,
				itemInfo.framebufferHeight,
				itemInfo.pMesh,
				subMeshIndex, 
				pPass, 
				pClipRect);
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
			itemInfo.pPreparingCommandBufferCostTimer->end();
#endif //DEBUG and VG_ENABLE_COST_TIMER
		}
		else
		{
			VG_LOG(plog::warning) << "No one valid shader module for pass. Pass ID: " << pPass->getID() << std::endl;
		}
    }

    void RenderPass::_createPipeline(vk::RenderPass *pRenderPass,
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

	void RenderPass::_recordCommandBuffer(vk::Pipeline *pPipeline,
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