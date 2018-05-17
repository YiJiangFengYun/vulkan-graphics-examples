#ifndef VG_RENDER_BINDER_HPP
#define VG_RENDER_BINDER_HPP

#include "graphics/global.hpp"
#include "graphics/material/material.hpp"
#include "graphics/scene/visual_object.hpp"
#include "graphics/scene/camera.hpp"
#include "graphics/scene/scene.hpp"
#include "graphics/post_render/post_render.hpp"
#include "graphics/renderer/renderer_option.hpp"
#include "graphics/renderer/pre_z_target.hpp"
#include "graphics/renderer/post_render_target.hpp"

namespace vg
{
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


        void bind(const BaseScene *pScene
            , const BaseCamera *pCamera
            , PreZTarget *pPreZTarget = nullptr
            , CmdBuffer *pPreZCmdBuffer = nullptr
			, CmdBuffer *pBranchCmdBuffer = nullptr
            , CmdBuffer *pTrunkWaitBarrierCmdBuffer = nullptr            
            , CmdBuffer *pTrunkRenderPassCmdBuffer = nullptr
            , PostRender *pPostRender = nullptr
            , PostRenderTarget *pPostRenderTarget = nullptr
            , CmdBuffer *pPostRenderCmdBuffer = nullptr
            );

    private:
        uint32_t m_framebufferWidth;
        uint32_t m_framebufferHeight;
        std::vector<BaseVisualObject *> m_bindedObjects;
		uint32_t m_bindedObjectCount;

        void _beginBind();

        void _bind(const BaseScene *pScene
            , const BaseCamera *pCamera
            , PreZTarget *pPreZTarget = nullptr
            , CmdBuffer *pPreZCmdBuffer = nullptr
			, CmdBuffer *pBranchCmdBuffer = nullptr
            , CmdBuffer *pTrunkWaitBarrierCmdBuffer = nullptr            
            , CmdBuffer *pTrunkRenderPassCmdBuffer = nullptr
            , PostRender *pPostRender = nullptr
            , PostRenderTarget *pPostRenderTarget = nullptr
            , CmdBuffer *pPostRenderCmdBuffer = nullptr
            );

        void _bindScene2(const Scene<SpaceType::SPACE_2> *pScene
		    , const Camera<SpaceType::SPACE_2> *pCamera
            , PreZTarget *pPreZTarget = nullptr
			, CmdBuffer *pPreZCmdBuffer = nullptr
			, CmdBuffer *pBranchCmdBuffer = nullptr
            , CmdBuffer *pTrunkRenderPassCmdBuffer = nullptr
            , CmdBuffer *pTrunkWaitBarrierCmdBuffer = nullptr
			);

		void _bindScene3(const Scene<SpaceType::SPACE_3> *pScene
		    , const Camera<SpaceType::SPACE_3> *pCamera
            , PreZTarget *pPreZTarget = nullptr
			, CmdBuffer *pPreZCmdBuffer = nullptr
			, CmdBuffer *pBranchCmdBuffer = nullptr
            , CmdBuffer *pTrunkRenderPassCmdBuffer = nullptr
            , CmdBuffer *pTrunkWaitBarrierCmdBuffer = nullptr
			);

        void _setBuildInData(BaseVisualObject * pVisualObject
            , Matrix4x4 modelMatrix
            , Matrix4x4 viewMatrix
            , Matrix4x4 projMatrix
            , PreZTarget *pPreZTarget
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
	    , fd::CostTimer * pPreparingBuildInDataCostTimer
#endif //DEBUG and VG_ENABLE_COST_TIMER
		);

		void _bindVisualObject(BaseVisualObject *pVisublObject
            , BaseVisualObject::BindInfo & bindInfo
            , BaseVisualObject::BindResult *pResult
            );

		void _endBind();
    };
} //vg

#endif //VG_RENDER_BINDER_HPP