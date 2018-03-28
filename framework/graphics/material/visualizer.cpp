#include "graphics/material/visualizer.hpp"

namespace vg
{
    Visualizer::BindInfo::BindInfo(const Matrix4x4 *pProjMatrix
        , const Matrix4x4 *pViewMatrix
        , PipelineCache *pPipelineCache
		, vk::RenderPass *pRenderPass
		, vk::CommandBuffer *pCommandBuffer
		, uint32_t framebufferWidth
		, uint32_t framebufferHeight
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        , fd::CostTimer *pPreparingBuildInDataCostTimer
        , fd::CostTimer *pPreparingPipelineCostTimer
        , fd::CostTimer *pPreparingCommandBufferCostTimer
#endif //DEBUG and VG_ENABLE_COST_TIMER
        , const Matrix4x4 *pModelMatrix
		, const uint32_t passCount
        , Pass * const *pPasses
        , const BaseMesh *pMesh
        , uint32_t subMeshOffset
        , uint32_t subMeshCount
        , Bool32 hasClipRect
        , const fd::Rect2D *pClipRects
        )
        : pProjMatrix(pProjMatrix)
        , pViewMatrix(pViewMatrix)
        , pPipelineCache(pPipelineCache)
		, pRenderPass(pRenderPass)
		, pCommandBuffer(pCommandBuffer)
		, framebufferWidth(framebufferWidth)
		, framebufferHeight(framebufferHeight)
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        , pPreparingBuildInDataCostTimer(pPreparingBuildInDataCostTimer)
        , pPreparingPipelineCostTimer(pPreparingPipelineCostTimer)
        , pPreparingCommandBufferCostTimer(pPreparingCommandBufferCostTimer)
#endif //DEBUG and VG_ENABLE_COST_TIMER
		, pModelMatrix(pModelMatrix)
        , passCount(passCount)
		, pPasses(pPasses)
        , pMesh(pMesh)
        , subMeshOffset(subMeshOffset)
        , subMeshCount(subMeshCount)
        , hasClipRect(hasClipRect)
        , pClipRects(pClipRects)
    {
    }

    Visualizer::BindResult::BindResult(uint32_t drawCount)
	    : drawCount(drawCount)
    {

    }

    Visualizer::Visualizer()
        : Base(BaseType::VISUALIZER)
    {

    }

    void Visualizer::bindToRender(const BindInfo info, BindResult *pResult)
    {
		uint32_t drawCount = 0u;

		// auto passCount = info.passCount;
		auto passCount = 1u;
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        info.pPreparingBuildInDataCostTimer->begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
		Bool32 hasMatrixObjectToNDC = VG_FALSE;
		Bool32 hasMatrixObjectToWorld = VG_FALSE;
		Bool32 hasMatrixObjectToView = VG_FALSE;
		Bool32 hasMatrixView = VG_FALSE;
		Bool32 hasMatrixProjection = VG_FALSE;
		for (uint32_t passIndex = 0u; passIndex < passCount; ++passIndex)
		{
			//update building in matrix variable.
			auto pPass = *(info.pPasses + passIndex);
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
		
        }
		Matrix4x4 mvMatrix;
		Matrix4x4 mvpMatrix;
		if (hasMatrixObjectToView || hasMatrixObjectToNDC)
		{
			mvMatrix = *info.pViewMatrix * *info.pModelMatrix;
        }
		
		if (hasMatrixObjectToNDC)
		{
			mvpMatrix = *info.pProjMatrix * mvMatrix;
		}
		auto pMesh = info.pMesh;
		auto pContentMesh = dynamic_cast<const ContentMesh *>(pMesh);
		auto subMeshOffset = info.subMeshOffset;
		auto subMeshCount = info.subMeshCount;
		
		for (uint32_t passIndex = 0u; passIndex < passCount; ++passIndex)
		{
			//update building in matrix variable.
			auto pPass = *(info.pPasses + passIndex);
			auto buildInDataInfo = pPass->getBuildInDataInfo();
			uint32_t componentCount = buildInDataInfo.componentCount;
			for (uint32_t componentIndex = 0u; componentIndex < componentCount; ++componentIndex)
			{
				Pass::BuildInDataType type = (*(buildInDataInfo.pComponent + componentIndex)).type;
				if (type == Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC)
				{
					pPass->_setBuildInMatrixData(type, mvpMatrix);
				}
				else if (type == Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD)
				{
					pPass->_setBuildInMatrixData(type, *info.pModelMatrix);
				}
				else if (type == Pass::BuildInDataType::MATRIX_OBJECT_TO_VIEW)
				{
					pPass->_setBuildInMatrixData(type, mvMatrix);
				}
				else if (type == Pass::BuildInDataType::MATRIX_VIEW)
				{
					pPass->_setBuildInMatrixData(type, *info.pViewMatrix);
				}
				else if (type == Pass::BuildInDataType::MATRIX_PROJECTION)
				{
					pPass->_setBuildInMatrixData(type, *info.pProjMatrix);
				}
			}
			pPass->apply();
		}

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        info.pPreparingBuildInDataCostTimer->end();
#endif //DEBUG and VG_ENABLE_COST_TIMER

		for (uint32_t subMeshIndex = 0u; subMeshIndex < subMeshCount; ++subMeshIndex)
		{
			for (uint32_t passIndex = 0u; passIndex < passCount; ++passIndex)
			{
				auto pPass = *(info.pPasses + passIndex);
				auto pShader = pPass->getShader();
				auto stageInfos = pShader->getShaderStageInfos();
				if (stageInfos.size() != 0)
				{
					std::shared_ptr<vk::Pipeline> pPipeline;
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                    info.pPreparingPipelineCostTimer->begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
					_createPipelineForObj(info.pRenderPass, 
					    info.pMesh, 
						subMeshIndex + subMeshOffset, 
						pPass, 
						info.pPipelineCache, 
						pPipeline);
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                    info.pPreparingPipelineCostTimer->end();
					info.pPreparingCommandBufferCostTimer->begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
                    const fd::Rect2D *pClipRect;
					if(info.hasClipRect)
					{
						pClipRect = (info.pClipRects + subMeshIndex);
					}
					else
					{
						pClipRect = nullptr;
					}
					_recordCommandBufferForObj(pPipeline.get(),
					    info.pCommandBuffer,
						info.framebufferWidth,
						info.framebufferHeight,
					    info.pMesh, 
						subMeshIndex + subMeshOffset, 
						pPass, 
						pClipRect);
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                    info.pPreparingCommandBufferCostTimer->end();
#endif //DEBUG and VG_ENABLE_COST_TIMER
				    ++drawCount;
				}
				else
				{
					VG_LOG(plog::warning) << "No one valid shader module for pass. Pass ID: " << pPass->getID() << std::endl;
				}
			}
		}

		pResult->drawCount = drawCount;
    }

	void Visualizer::_createPipelineForObj(vk::RenderPass *pRenderPass,
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

	void Visualizer::_recordCommandBufferForObj(vk::Pipeline *pPipeline,
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