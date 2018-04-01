#include "graphics/material/render_pass_info.hpp"

namespace vg
{
    RenderPassInfo::RenderPassInfo( vk::RenderPass *pRenderPass
        , vk::Framebuffer *pFrameBuffer
        , uint32_t framebufferWidth
        , uint32_t framebufferHeight
        , fd::Rect2D renderArea
        , uint32_t clearValueCount
        , vk::ClearValue *pClearValues
        , Matrix4x4 projMatrix
        , Matrix4x4 viewMatrix
        , Pass *pPass
        , Matrix4x4 modelMatrix
        , const BaseMesh *pMesh
        , uint32_t subMeshIndex
        , Bool32 hasClipRect
        , const fd::Rect2D *pClipRect
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        , fd::CostTimer *pPreparingBuildInDataCostTimer
        , fd::CostTimer *pPreparingPipelineCostTimer
        , fd::CostTimer *pPreparingCommandBufferCostTimer
#endif //DEBUG and VG_ENABLE_COST_TIMER
        )
        : pRenderPass(pRenderPass)
        , pFrameBuffer(pFrameBuffer)
	, framebufferWidth(framebufferWidth)
	, framebufferHeight(framebufferHeight)
        , renderArea(renderArea)
        , clearValueCount(clearValueCount)
        , pClearValues(pClearValues)
        , projMatrix(projMatrix)
        , viewMatrix(viewMatrix)
        , pPass(pPass)
        , modelMatrix(modelMatrix)
        , pMesh(pMesh)
        , subMeshIndex(subMeshIndex)
        , hasClipRect(hasClipRect)
        , pClipRect(pClipRect)
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        , pPreparingBuildInDataCostTimer(pPreparingBuildInDataCostTimer)
        , pPreparingPipelineCostTimer(pPreparingPipelineCostTimer)
        , pPreparingCommandBufferCostTimer(pPreparingCommandBufferCostTimer)
#endif //DEBUG and VG_ENABLE_COST_TIMER
        {

        }
} //vg