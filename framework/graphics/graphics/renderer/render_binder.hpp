#ifndef VG_RENDER_BINDER_HPP
#define VG_RENDER_BINDER_HPP

#include "graphics/global.hpp"
#include "graphics/material/material.hpp"
#include "graphics/scene/visual_object.hpp"
#include "graphics/scene/projector.hpp"
#include "graphics/scene/scene.hpp"
#include "graphics/post_render/post_render.hpp"
#include "graphics/renderer/renderer_option.hpp"
#include "graphics/render_target/render_target.hpp"
#include "graphics/render_target/pre_depth_target.hpp"
#include "graphics/render_target/post_render_target.hpp"
#include "graphics/util/frame_object_cache.hpp"

namespace vg
{
    struct RenderBinderInfo {
        Bool32 lightingEnable;
        Bool32 shadowEnable;
        Bool32 preDepthEnable;
        Bool32 postRenderEnable;

        Bool32 firstScene;
        BaseScene *pScene;
        const BaseProjector *pProjector;
        PostRender *pPostRender;

        const PreDepthTarget *pPreDepthTarget;
        const PostRenderTarget *pPostRenderTarget;
        const MultiRenderTarget *pRendererTarget;

        const Texture *pPreDepthResultTex;
        const Texture *pPostRenderTex;

        CmdBuffer *pLightDepthCmdBuffer;
        CmdBuffer *pPreDepthCmdBuffer;
        CmdBuffer *pBranchCmdBuffer;
        CmdBuffer *pTrunkWaitBarrierCmdBuffer;
        CmdBuffer *pTrunkRenderPassCmdBuffer;
        CmdBuffer *pPostRenderCmdBuffer;

        RenderBinderInfo(Bool32 lightingEnable = VG_FALSE
            , Bool32 shadowEnable = VG_FALSE
            , Bool32 preDepthEnable = VG_FALSE
            , Bool32 postRenderEnable = VG_FALSE

            , Bool32 firstScene = VG_TRUE
            , BaseScene *pScene = nullptr
            , const BaseProjector *pProjector = nullptr
            , PostRender *pPostRender = nullptr
    
            , const PreDepthTarget *pPreDepthTarget = nullptr
            , const PostRenderTarget *pPostRenderTarget = nullptr
            , const MultiRenderTarget *pRendererTarget = nullptr

            , const Texture *pPreDepthResultTex = nullptr
            , const Texture *pPostRenderTex = nullptr

            , CmdBuffer *pLightDepthCmdBuffer = nullptr
            , CmdBuffer *pPreDepthCmdBuffer = nullptr
            , CmdBuffer *pBranchCmdBuffer = nullptr
            , CmdBuffer *pTrunkWaitBarrierCmdBuffer = nullptr
            , CmdBuffer *pTrunkRenderPassCmdBuffer = nullptr
            , CmdBuffer *pPostRenderCmdBuffer = nullptr
            );
    };
    class RenderBinder 
    {
    public:
        RenderBinder(uint32_t framebufferWidth = 0u
            , uint32_t framebufferHeight = 0u
            );
        uint32_t getFramebufferWidth() const;
        void setFramebufferWidth(uint32_t value);
        uint32_t getFramebufferHeight() const;
        void setFramebufferHeight(uint32_t value);

        void begin();

        void bind(RenderBinderInfo info);
        
        void end();

    private:
        uint32_t m_framebufferWidth;
        uint32_t m_framebufferHeight;
        std::vector<BaseVisualObject *> m_bindedObjects;
        uint32_t m_bindedObjectCount;

        //light data buffer.
        FrameObjectCache<InstanceID, std::shared_ptr<BufferData>> m_lightDataBufferCache;
        BufferData *m_pCurrLightDataBuffer;
        uint32_t m_lightTypeCount;
        std::vector<PassTextureInfo> m_lightPassTextureInfos;
        std::vector<std::vector<PassTextureInfo::TextureInfo>> m_lightTextureInfos;

        void _beginBind();

        void _bind(RenderBinderInfo info);

        void _endBind();

        void _bindForLightDepth(Scene<SpaceType::SPACE_3> *pScene
            , const Projector<SpaceType::SPACE_3> *pProjector
            , CmdBuffer *pPreDepthCmdBuffer = nullptr);

        void _syncLightData(BaseScene *pScene);

        void _bindForRenderPassBegin(RenderBinderInfo info);

        void _bindForRenderPassEnd(RenderBinderInfo info);

        void _bindScene2(Scene<SpaceType::SPACE_2> *pScene
            , const Projector<SpaceType::SPACE_2> *pProjector
            , const Texture *pPreDepthResultTex = nullptr
            , CmdBuffer *pPreDepthCmdBuffer = nullptr
            , CmdBuffer *pBranchCmdBuffer = nullptr
            , CmdBuffer *pTrunkWaitBarrierCmdBuffer = nullptr
            , CmdBuffer *pTrunkRenderPassCmdBuffer = nullptr
            );

        void _bindScene3(Scene<SpaceType::SPACE_3> *pScene
            , const Projector<SpaceType::SPACE_3> *pProjector
            , const Texture *pPreDepthResultTex = nullptr
            , CmdBuffer *pPreDepthCmdBuffer = nullptr
            , CmdBuffer *pBranchCmdBuffer = nullptr
            , CmdBuffer *pTrunkWaitBarrierCmdBuffer = nullptr
            , CmdBuffer *pTrunkRenderPassCmdBuffer = nullptr
            );

        void _setPreDepthBuildInData(BaseVisualObject * pVisualObject
            , Matrix4x4 modelMatrix
            , Matrix4x4 viewMatrix
            , Matrix4x4 projMatrix
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
            , fd::CostTimer * pPreparingBuildInDataCostTimer
#endif //DEBUG and VG_ENABLE_COST_TIMER
            );

        void _setBuildInData(BaseVisualObject * pVisualObject
            , Matrix4x4 modelMatrix
            , Matrix4x4 viewMatrix
            , Matrix4x4 projMatrix
            , BaseScene *pScene            
            , const Texture *pPreDepthResultTex
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        , fd::CostTimer * pPreparingBuildInDataCostTimer
#endif //DEBUG and VG_ENABLE_COST_TIMER
        );

        void _bindVisualObject(BaseVisualObject *pVisublObject
            , BaseVisualObject::BindInfo & bindInfo
            , BaseVisualObject::BindResult *pResult
            );

        void _renderPassBegin(const BaseRenderTarget *pRenderTarget
            , const vk::RenderPass *pRenderPass
            , const vk::Framebuffer *pFramebuffer
            , CmdBuffer *pCmdBuffer
            );
        void _renderPassEnd(CmdBuffer *pCmdBuffer);
    };
} //vg

#endif //VG_RENDER_BINDER_HPP