#ifndef VG_RENDER_PASS_HPP
#define VG_RENDER_PASS_HPP

#include "graphics/global.hpp"
#include "graphics/module.hpp"
#include "graphics/renderer/pipeline_cache.hpp"
#include "graphics/mesh/mesh.hpp"
#include "graphics/buffer_data/util.hpp"
#include "graphics/material/cmd.hpp"

namespace vg
{
    class RenderPass
    {
    public:
        struct ResultInfo
        {
			ResultInfo();
        };

        static void recordTrunk(CmdBuffer *pCmdBuffer
            , vk::CommandBuffer *pCommandBuffer
            , PipelineCache *pPipelineCache
            , vk::RenderPass *pRenderPass
            , ResultInfo *pResult = nullptr
            );
        
        static void recordBranch(CmdBuffer *pCmdBuffer
            , vk::CommandBuffer *pCommandBuffer
            , PipelineCache *pPipelineCache
            , ResultInfo *pResult = nullptr
            );

        static void recordItemRenderPassBegin(RenderPassInfo *pRenderPassInfo
            ,  vk::CommandBuffer *pCommandBuffer);
        static void recordItemRenderPassEnd(RenderPassInfo *pRenderPassInfo
            ,  vk::CommandBuffer *pCommandBuffer);

        static void recordItem(RenderPassInfo *pRenderPassInfo
            ,  vk::CommandBuffer *pCommandBuffer
            , PipelineCache *pPipelineCache
            , ResultInfo *pResult = nullptr);

		static void _createPipeline(vk::RenderPass *pRenderPass,
		    const BaseMesh *pMesh,
		    uint32_t subMeshIndex,
		    Pass *pPass,
		    PipelineCache *pPipelineCache,
		    std::shared_ptr<vk::Pipeline> &pPipeline);

		static void _recordCommandBuffer(vk::Pipeline *pPipeline,
	        vk::CommandBuffer *pCommandBuffer,
		    uint32_t framebufferWidth,
		    uint32_t framebufferHeight,
		    const BaseMesh *pMesh,
		    uint32_t subMeshIndex,
		    Pass *pPass,
		    const fd::Rect2D *pClipRect);
    };
} //vg

#endif