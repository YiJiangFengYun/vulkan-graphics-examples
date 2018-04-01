#include "graphics/material/visualizer.hpp"

namespace vg
{
    Visualizer::BindInfo::BindInfo(const Matrix4x4 *pProjMatrix
        , const Matrix4x4 *pViewMatrix
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

    Visualizer::BindResult::BindResult(CmdBuffer *pBranchCmdBuffer
        , CmdBuffer *pTrunkRenderPassCmdBuffer
        , CmdBuffer *pTrunkWaitBarrierCmdBuffer
        )
	    : pBranchCmdBuffer(pBranchCmdBuffer)
        , pTrunkRenderPassCmdBuffer(pTrunkRenderPassCmdBuffer)
        , pTrunkWaitBarrierCmdBuffer(pTrunkWaitBarrierCmdBuffer)
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

        if (result.pTrunkRenderPassCmdBuffer != nullptr)
        {
            RenderPassInfo trunkRenderPassInfo;
            trunkRenderPassInfo.pRenderPass = nullptr;
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

            CmdInfo cmdInfo;
            cmdInfo.pRenderPassInfo = &trunkRenderPassInfo;

            result.pTrunkRenderPassCmdBuffer->addCmd(cmdInfo);
        }

        
        
    }
} //vg