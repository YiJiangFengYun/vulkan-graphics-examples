#ifndef VG_VISUALIZER_HPP
#define VG_VISUALIZER_HPP

#include "graphics/global.hpp"
#include "graphics/module.hpp"
#include "graphics/base.hpp"
#include "graphics/mesh/mesh.hpp"
#include "graphics/buffer_data/util.hpp"
#include "graphics/material/render_pass_info.hpp"

namespace vg
{
    class Visualizer : public Base
    {
    public:    
        struct BindInfo 
        {
            const Matrix4x4 *pProjMatrix;
            const Matrix4x4 *pViewMatrix;
			uint32_t trunkFramebufferWidth;
			uint32_t trunkFramebufferHeight;
            const Matrix4x4 *pModelMatrix;
            const BaseMesh *pMesh;
            uint32_t subMeshIndex;
            Bool32 hasClipRect;
            const fd::Rect2D *pClipRect;
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
            fd::CostTimer *pPreparingBuildInDataCostTimer;
            fd::CostTimer *pPreparingPipelineCostTimer;
            fd::CostTimer *pPreparingCommandBufferCostTimer;
#endif //DEBUG and VG_ENABLE_COST_TIMER

            BindInfo(const Matrix4x4 *pProjMatrix = nullptr
                , const Matrix4x4 *pViewMatrix = nullptr
			    , uint32_t trunkFramebufferWidth = 0u
			    , uint32_t trunkFramebufferHeight = 0u
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                , fd::CostTimer *pPreparingBuildInDataCostTimer = nullptr
                , fd::CostTimer *pPreparingPipelineCostTimer = nullptr
                , fd::CostTimer *pPreparingCommandBufferCostTimer = nullptr
#endif //DEBUG and VG_ENABLE_COST_TIMER
                , const Matrix4x4 *pModelMatrix = nullptr
                , const BaseMesh *pMesh = nullptr
                , uint32_t subMeshIndex = 0u
                , Bool32 hasClipRect = VG_FALSE
                , const fd::Rect2D *pClipRect = nullptr
                );
        };
    
        struct BindResult
        {
            uint32_t branchRenderPassCount;
            RenderPassInfo *pBranchRenderPassInfos;
            uint32_t trunkRenderPassCount;
            RenderPassInfo *pTrunkRenderPassInfos;
            uint32_t trunkWaitBarrierCount;
            TrunkWaitBarrierInfo *pTrunkWaitBarrierInos;
            BindResult(uint32_t branchRenderPassCount = 0u
                , RenderPassInfo *pBranchRenderPassInfos = nullptr
                , uint32_t trunkRenderPassCount = 0u
                , RenderPassInfo *pTrunkRenderPassInfos = nullptr
                , uint32_t trunkWaitBarrierCount = 0u
                , TrunkWaitBarrierInfo *pTrunkWaitBarrierInos = nullptr
                );
        };

        Visualizer(uint32_t passCount, Pass * const *pPasses);

        void updatePassInfo(uint32_t passCount, Pass * const *pPasses);

        void bindToRender(const BindInfo info, BindResult *pResult);
        

    protected:
        Visualizer() = delete;
        uint32_t m_passCount;
        Pass * const *m_pPasses;

    private:
    };
} //vg

#endif //VG_VISUALIZER_HPP