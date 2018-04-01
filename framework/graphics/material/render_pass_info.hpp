#ifndef VG_RENDER_PASS_INFO_HPP
#define VG_RENDER_PASS_INFO_HPP

#include "graphics/global.hpp"
#include "graphics/pass/pass.hpp"
#include "graphics/mesh/mesh.hpp"

namespace vg
{
    struct TrunkWaitBarrierInfo
    {
        vk::PipelineStageFlags  srcStageMask; 
        vk::PipelineStageFlags  dstStageMask;
    };

    struct RenderPassInfo
    {
        vk::RenderPass *pRenderPass;
        vk::Framebuffer *pFrameBuffer;
        uint32_t framebufferWidth;
		uint32_t framebufferHeight;
        fd::Rect2D renderArea;
        uint32_t clearValueCount;
        vk::ClearValue *pClearValues;

        Matrix4x4 projMatrix;
        Matrix4x4 viewMatrix;
        Pass *pPass;
        Matrix4x4 modelMatrix;
        const BaseMesh *pMesh;
		uint32_t subMeshIndex;
        Bool32 hasClipRect;
        const fd::Rect2D *pClipRect;
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        fd::CostTimer *pPreparingBuildInDataCostTimer;
        fd::CostTimer *pPreparingPipelineCostTimer;
        fd::CostTimer *pPreparingCommandBufferCostTimer;
#endif //DEBUG and VG_ENABLE_COST_TIMER
            
        RenderPassInfo( vk::RenderPass *pRenderPass = nullptr
            , vk::Framebuffer *pFrameBuffer = nullptr
			, uint32_t framebufferWidth = 0u
			, uint32_t framebufferHeight = 0u
            , fd::Rect2D renderArea = fd::Rect2D()
            , uint32_t clearValueCount = 0u
            , vk::ClearValue *pClearValues = nullptr
            , Matrix4x4 projMatrix = Matrix4x4(1.0f)
            , Matrix4x4 viewMatrix = Matrix4x4(1.0f)
            , Pass *pPass = nullptr
            , Matrix4x4 modelMatrix = Matrix4x4(1.0f)
            , const BaseMesh *pMesh = nullptr
            , uint32_t subMeshIndex = 0u
            , Bool32 hasClipRect = VG_FALSE
            , const fd::Rect2D *pClipRect = nullptr
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
            , fd::CostTimer *pPreparingBuildInDataCostTimer = nullptr
            , fd::CostTimer *pPreparingPipelineCostTimer = nullptr
            , fd::CostTimer *pPreparingCommandBufferCostTimer = nullptr
#endif //DEBUG and VG_ENABLE_COST_TIMER
            );
    };
} //vg

#endif //VG_RENDER_PASS_INFO_HPP