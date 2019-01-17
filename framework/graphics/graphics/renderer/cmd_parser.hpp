#ifndef VG_CMD_PARSER_HPP
#define VG_CMD_PARSER_HPP

#include "graphics/global.hpp"
#include "graphics/app/app.hpp"
#include "graphics/renderer/pipeline_cache.hpp"
#include "graphics/renderer/renderer_pass.hpp"
#include "graphics/mesh/mesh.hpp"
#include "graphics/buffer_data/util.hpp"
#include "graphics/material/cmd.hpp"

namespace vg
{
    class CMDParser
    {
    public:
        struct ResultInfo
        {
            uint32_t drawCount;
            ResultInfo(uint32_t drawCount = 0u);
        };

        static void record(CmdBuffer *pCmdBuffer
            , vk::CommandBuffer *pCommandBuffer
            , PipelineCache *pPipelineCache
            , RendererPassCache *pRendererPassCache
            , ResultInfo *pResult = nullptr
            );
            
        static void recordTrunkWaitBarrier(CmdBuffer *pTrunkWaitBarrierCmdBuffer
            , vk::CommandBuffer *pCommandBuffer
            );

        static void recordItemRenderPassBegin(const RenderPassBeginInfo *pRenderPassBeginInfo
            ,  vk::CommandBuffer *pCommandBuffer
            );
        static void recordItemRenderPassEnd(const RenderPassEndInfo *pRenderPassEndInfo
            ,  vk::CommandBuffer *pCommandBuffer);
        static void recordItemRenderPassEnd(vk::CommandBuffer *pCommandBuffer);

        static void recordItemNextSubpass(vk::CommandBuffer *pCommandBuffer);

        static void recordItem(const RenderPassInfo *pRenderPassInfo
            , vk::CommandBuffer *pCommandBuffer
            , PipelineCache *pPipelineCache
            , RendererPassCache *pRendererPassCache
            , ResultInfo *pResult = nullptr);

        static void _createPipeline(const vk::RenderPass *pRenderPass,
            const BaseMesh *pMesh,
            uint32_t subMeshIndex,
            const Pass *pPass,
            const RendererPass *pRendererPass,
            PipelineCache *pPipelineCache,
            std::shared_ptr<vk::Pipeline> &pPipeline);

        static void _recordCommandBuffer(vk::Pipeline *pPipeline,
            vk::CommandBuffer *pCommandBuffer,
            uint32_t framebufferWidth,
            uint32_t framebufferHeight,
            const BaseMesh *pMesh,
            uint32_t subMeshIndex,
            const Pass *pPass,
            const RendererPass *pRendererPass,
            const fd::Viewport viewport,
            const fd::Rect2D scissor,
            const CmdDraw * pCmdDraw,
            const CmdDrawIndexed * pCmdDrawIndexed
        );
    };
} //vg

#endif //VG_CMD_PARSER_HPP