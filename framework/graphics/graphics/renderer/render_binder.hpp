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
#include "graphics/render_target/pre_z_target.hpp"
#include "graphics/render_target/post_render_target.hpp"
#include "graphics/util/frame_object_cache.hpp"

namespace vg
{
    struct RenderBinderInfo {
        Bool32 firstScene;
        Bool32 preZEnable;
        Bool32 postRenderEnable;

        BaseScene *pScene;
        const BaseProjector *pProjector;
        PostRender *pPostRender;

        const PreZTarget *pPreZTarget;
        const PostRenderTarget *pPostRenderTarget;
        const MultiRenderTarget *pRendererTarget;

        const Texture *pPreZResultTex;
        const Texture *pPostRenderTex;

        CmdBuffer *pPreZCmdBuffer;
        CmdBuffer *pBranchCmdBuffer;
        CmdBuffer *pTrunkWaitBarrierCmdBuffer;
        CmdBuffer *pTrunkRenderPassCmdBuffer;
        CmdBuffer *pPostRenderCmdBuffer;

        RenderBinderInfo(Bool32 firstScene = VG_TRUE
            , Bool32 preZEnable = VG_FALSE
            , Bool32 postRenderEnable = VG_FALSE
            , BaseScene *pScene = nullptr
            , const BaseProjector *pProjector = nullptr
            , PostRender *pPostRender = nullptr
    
            , const PreZTarget *pPreZTarget = nullptr
            , const PostRenderTarget *pPostRenderTarget = nullptr
            , const MultiRenderTarget *pRendererTarget = nullptr

            , const Texture *pPreZResultTex = nullptr
            , const Texture *pPostRenderTex = nullptr
    
            , CmdBuffer *pPreZCmdBuffer = nullptr
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

        void _syncLightData(BaseScene *pScene);

        void _bindForRenderPassBegin(RenderBinderInfo info);

        void _bindForRenderPassEnd(RenderBinderInfo info);

        void _bindScene2(Scene<SpaceType::SPACE_2> *pScene
            , const Projector<SpaceType::SPACE_2> *pProjector
            , const Texture *pPreZResultTex = nullptr
            , CmdBuffer *pPreZCmdBuffer = nullptr
            , CmdBuffer *pBranchCmdBuffer = nullptr
            , CmdBuffer *pTrunkWaitBarrierCmdBuffer = nullptr
            , CmdBuffer *pTrunkRenderPassCmdBuffer = nullptr
            );

        void _bindScene3(Scene<SpaceType::SPACE_3> *pScene
            , const Projector<SpaceType::SPACE_3> *pProjector
            , const Texture *pPreZResultTex = nullptr
            , CmdBuffer *pPreZCmdBuffer = nullptr
            , CmdBuffer *pBranchCmdBuffer = nullptr
            , CmdBuffer *pTrunkWaitBarrierCmdBuffer = nullptr
            , CmdBuffer *pTrunkRenderPassCmdBuffer = nullptr
            );

        void _setPreZBuildInData(BaseVisualObject * pVisualObject
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
            , const Texture *pPreZResultTex
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