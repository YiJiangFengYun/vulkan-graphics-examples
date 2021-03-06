#ifndef VG_RENDERER_H
#define VG_RENDERER_H

#include "graphics/global.hpp"
#include "graphics/scene/space_info.hpp"
#include "graphics/scene/scene.hpp"
#include "graphics/scene/camera.hpp"
#include "graphics/renderer/renderer_option.hpp"
#include "graphics/texture/texture_color_attachment.hpp"
#include "graphics/texture/texture_depth_stencil_attachment.hpp"
#include "graphics/texture/texture_2d.hpp"
#include "graphics/renderer/pipeline_cache.hpp"
#include "graphics/renderer/renderer_target.hpp"
#include "graphics/post_render/post_render.hpp"
#include "graphics/renderer/renderer_pre_depth_target.hpp"
#include "graphics/renderer/renderer_post_render_target.hpp"
#include "graphics/renderer/render_binder.hpp"
#include "graphics/renderer/renderer_pass.hpp"

//todo: batch mesh,
//todo: cache graphics pipeline.

namespace vg
{
    
    class Renderer : public Base
    {
    public:
        struct SceneInfo {
            const BaseScene *pScene;
            const BaseCamera *pCamera;
            Bool32 preDepth;
            const PostRender * pPostRender;
            SceneInfo(const BaseScene *pScene = nullptr
                , const BaseCamera *pCamera = nullptr
                , Bool32 preDepth = VG_FALSE
                , const PostRender * pPostRender = nullptr);
        };

        struct RenderInfo {
            const SceneInfo *pSceneInfos;
            uint32_t sceneInfoCount;
            uint32_t waitSemaphoreCount;
            const vk::Semaphore* pWaitSemaphores;
            const vk::PipelineStageFlags* pWaitDstStageMask;
            uint32_t signalSemaphoreCount;
            const vk::Semaphore* pSignalSemaphores;

            RenderInfo(uint32_t sceneInfoCount = 0u
                , const SceneInfo *pSceneInfos = nullptr
                , uint32_t waitSemaphoreCount = 0u
                , const vk::Semaphore* pWaitSemaphores = nullptr
                , const vk::PipelineStageFlags* pWaitDstStageMask = nullptr
                , uint32_t signalSemaphoreCount = 0u
                , const vk::Semaphore* pSignalSemaphores = nullptr
                );
        };

        struct RenderResultInfo {
            Bool32 isRendered;
            uint32_t drawCount;

            RenderResultInfo(Bool32 isRendered = VG_FALSE
                , uint32_t drawCount = 0u);
        };

        Renderer(const RendererTarget * pRendererTarget = nullptr);
        ~Renderer();

        const RendererTarget * getRendererTarget() const;
        void setRendererTarget(const RendererTarget * pRendererTarget);

        void enableLighting();
        void disableLighting();

        void enableShadow();
        void disableShadow();

        void enablePreDepth();
        void disablePreDepth();

        void enablePostRender();
        void disablePostRender();

        Bool32 isValidForRender() const;

        // void renderBegin();
        void render(const RenderInfo &info, RenderResultInfo &resultInfo);
        // void renderEnd(const RenderInfo &info);

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        const fd::CostTimer &getPreparingRenderCostTimer() const;
#endif //DEBUG and VG_ENABLE_COST_TIMER
    protected:
        uint32_t m_framebufferWidth;
        uint32_t m_framebufferHeight;

        const RendererTarget *m_pRendererTarget;

        std::shared_ptr<vk::CommandPool> m_pCommandPool;
        std::shared_ptr<vk::CommandBuffer> m_pCommandBuffer;

        PipelineCache m_pipelineCache;
        RendererPassCache m_rendererPassCache;
        
        RenderBinder m_renderBinder;

        // light shadow.
        Bool32 m_lightingEnable;
        Bool32 m_shadowEnable;
        std::shared_ptr<CmdBuffer> m_pLightDepthCmdBuffer;

        //pre z pass
        Bool32 m_preDepthEnable;
        std::shared_ptr<RendererPreDepthTarget> m_pPreDepthTarget;
        std::shared_ptr<CmdBuffer> m_pPreDepthCmdBuffer;

        //post render
        Bool32 m_postRenderEnable;
        std::shared_ptr<RendererPostRenderTarget> m_pPostRenderTarget;
        std::shared_ptr<CmdBuffer> m_pPostRenderCmdbuffer;
        
        CmdBuffer m_trunkRenderPassCmdBuffer;
        CmdBuffer m_trunkWaitBarrierCmdBuffer;
        CmdBuffer m_branchCmdBuffer;

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        fd::CostTimer m_preparingRenderCostTimer;
#endif //DEBUG and VG_ENABLE_COST_TIMER

        void _resetFramebufferSize(uint32_t width, uint32_t height);

        virtual void _preRender();
        virtual void _renderBegin(const RenderInfo & info, RenderResultInfo & resultInfo);
        virtual void _render(const RenderInfo & info, RenderResultInfo & resultInfo);
        virtual void _renderEnd(const RenderInfo & info, RenderResultInfo & resultInfo);
        virtual void _postRender();
        virtual Bool32 _isValidForRender() const;

        virtual void _renderScene(const SceneInfo &sceneInfo, Bool32 isFirstScene, RenderResultInfo &resultInfo);

        
        void _createCommandPool();
        void _createCommandBuffer();
        
        //void _createFence();

        void _recordCommandBufferForBegin();
        void _recordCommandBufferForEnd();
        
        void _createLightingObjs();
        void _destroyLightingObjs();
        void _createPreDepthObjs();
        void _destroyPreDepthObjs();
        void _createPostRenderObjs();
        void _destroyPostRenderObjs();

    private:
    };

    
} //namespace kgs

#endif // !VG_RENDERER_H
