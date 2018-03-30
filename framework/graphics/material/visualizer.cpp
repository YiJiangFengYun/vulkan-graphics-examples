#include "graphics/material/visualizer.hpp"

namespace vg
{
    Visualizer::BindInfo::BindInfo(const Matrix4x4 *pProjMatrix
        , const Matrix4x4 *pViewMatrix
        , vk::RenderPass *pTrunkRenderPass
		, uint32_t trunkFramebufferWidth
		, uint32_t trunkFramebufferHeight
        , const Matrix4x4 *pModelMatrix
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
        : pProjMatrix(pProjMatrix)
        , pViewMatrix(pViewMatrix)
		, pTrunkRenderPass(pTrunkRenderPass)
		, trunkFramebufferWidth(trunkFramebufferWidth)
		, trunkFramebufferHeight(trunkFramebufferHeight)
		, pModelMatrix(pModelMatrix)
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

    Visualizer::BindResult::BindResult(uint32_t branchRenderPassCount
        , RenderPassInfo *pBranchRenderPasses
        , uint32_t trunkRenderPassCount
        , RenderPassInfo *pTrunkRenderPasses
        )
	    : branchRenderPassCount(branchRenderPassCount)
        , pBranchRenderPasses(pBranchRenderPasses)
        , trunkRenderPassCount(trunkRenderPassCount)
        , pTrunkRenderPasses(pTrunkRenderPasses)
    {

    }

    Visualizer::Visualizer(uint32_t passCount, Pass * const *pPasses)
        : Base(BaseType::VISUALIZER)
		, m_passCount(passCount)
		, m_pPasses(pPasses)
    {

    }

	void Visualizer::updatePassInfo(uint32_t passCount, Pass * const *pPasses)
	{
		m_passCount = passCount;
		m_pPasses = pPasses;
	}

    void Visualizer::bindToRender(const BindInfo info, BindResult *pResult)
    {
        if (m_passCount == 0) throw std::runtime_error("Pass of visualizer is empty.");
        if (m_passCount > 1) throw std::runtime_error("Visualizer don't support passes more than 1.");
		auto &result = *pResult;
        result.branchRenderPassCount = 0u;

        if (result.pBranchRenderPasses != nullptr)
        {

        }
        
        result.trunkRenderPassCount = 1u;

        if (result.pTrunkRenderPasses != nullptr)
        {
            auto &trunkRenderPassInfo = *result.pTrunkRenderPasses;
            trunkRenderPassInfo.pRenderPass = info.pTrunkRenderPass;
            trunkRenderPassInfo.framebufferWidth = info.trunkFramebufferWidth;
            trunkRenderPassInfo.framebufferHeight = info.trunkFramebufferHeight;
            trunkRenderPassInfo.projMatrix = *(info.pProjMatrix);
            trunkRenderPassInfo.viewMatrix = *(info.pViewMatrix);
            trunkRenderPassInfo.pPass = * m_pPasses;
            trunkRenderPassInfo.modelMatrix = *(info.pModelMatrix);
            trunkRenderPassInfo.pMesh = info.pMesh;
            trunkRenderPassInfo.subMeshIndex = info.subMeshIndex;
            trunkRenderPassInfo.hasClipRect = info.hasClipRect;
            trunkRenderPassInfo.pClipRect = info.pClipRect;
        }
        
    }
} //vg