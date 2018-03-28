#ifndef VG_VISUALIZER_HPP
#define VG_VISUALIZER_HPP

#include "graphics/global.hpp"
#include "graphics/module.hpp"
#include "graphics/base.hpp"
#include "graphics/pipeline/pipeline_cache.hpp"
#include "graphics/mesh/mesh.hpp"
#include "graphics/buffer_data/util.hpp"

namespace vg
{
    class Visualizer : public Base
    {
    public:    
        struct BindInfo 
        {
            const Matrix4x4 *pProjMatrix;
            const Matrix4x4 *pViewMatrix;
            PipelineCache *pPipelineCache;
            vk::RenderPass *pRenderPass;
            vk::CommandBuffer *pCommandBuffer;
			uint32_t framebufferWidth;
			uint32_t framebufferHeight;
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
            fd::CostTimer *pPreparingBuildInDataCostTimer;
            fd::CostTimer *pPreparingPipelineCostTimer;
            fd::CostTimer *pPreparingCommandBufferCostTimer;
#endif //DEBUG and VG_ENABLE_COST_TIMER
            const Matrix4x4 *pModelMatrix;
            const uint32_t passCount;
            Pass * const *pPasses;
            const BaseMesh *pMesh;
            uint32_t subMeshOffset;
            uint32_t subMeshCount;
            Bool32 hasClipRect;
            const fd::Rect2D *pClipRects;

            BindInfo(const Matrix4x4 *pProjMatrix = nullptr
                , const Matrix4x4 *pViewMatrix = nullptr
                , PipelineCache *pPipelineCache = nullptr
                , vk::RenderPass *pRenderPass = nullptr
                , vk::CommandBuffer *pCommandBuffer = nullptr
			    , uint32_t framebufferWidth = 0u
			    , uint32_t framebufferHeight = 0u
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                , fd::CostTimer *pPreparingBuildInDataCostTimer = nullptr
                , fd::CostTimer *pPreparingPipelineCostTimer = nullptr
                , fd::CostTimer *pPreparingCommandBufferCostTimer = nullptr
#endif //DEBUG and VG_ENABLE_COST_TIMER
                , const Matrix4x4 *pModelMatrix = nullptr
                , const uint32_t passCount = 0u
                , Pass * const *pPasses = nullptr
                , const BaseMesh *pMesh = nullptr
                , uint32_t subMeshOffset = 0u
                , uint32_t subMeshCount = 0u
                , Bool32 hasClipRect = VG_FALSE
                , const fd::Rect2D *pClipRects = nullptr
                );
        };
    
        struct BindResult
        {
            uint32_t drawCount;
            BindResult(uint32_t drawCount = 0u);
        };

        Visualizer();

        void bindToRender(const BindInfo info, BindResult *pResult);
        

    protected:
        void _createPipelineForObj(vk::RenderPass *pRenderPass,
		    const BaseMesh *pMesh,
		    uint32_t subMeshIndex,
		    Pass *pPass,
		    PipelineCache *pPipelineCache,
		    std::shared_ptr<vk::Pipeline> &pPipeline);

		void _recordCommandBufferForObj(vk::Pipeline *pPipeline,
	        vk::CommandBuffer *pCommandBuffer,
		    uint32_t framebufferWidth,
		    uint32_t framebufferHeight,
		    const BaseMesh *pMesh,
		    uint32_t subMeshIndex,
		    Pass *pPass,
		    const fd::Rect2D *pClipRect);
    private:

    };
} //vg

#endif //VG_VISUALIZER_HPP