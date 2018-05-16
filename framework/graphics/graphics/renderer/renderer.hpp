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
#include "graphics/renderer/render_target.hpp"
#include "graphics/post_render/post_render.hpp"

//todo: batch mesh,
//todo: cache graphics pipeline.

namespace vg
{
	
	class Renderer : public Base
	{
	public:
		struct SceneInfo {
			BaseScene *pScene;
			BaseCamera *pCamera;
			Bool32 preZ;
			PostRender * postRender;
			SceneInfo(BaseScene *pScene = nullptr
			    , BaseCamera *pCamera = nullptr
				, Bool32 preZ = VG_FALSE
				, PostRender * postRender = nullptr);
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

		static const vk::Format DEFAULT_PRE_Z_DEPTH_FORMAT;
		static const vk::Format DEFAULT_POST_RENDER_COLOR_FORMAT;
		static const vk::Format DEFAULT_POST_RENDER_DEPTH_STENCIL_FORMAT;

		Renderer(const RenderTarget * pRenderTarget = nullptr);
		~Renderer();

        const RenderTarget * getRenderTarget() const;
		void setRenderTarget(const RenderTarget * pRenderTarget);

		void enablePreZ();
		void disablePreZ();

		void enablePostRender();
		void disablePostRender();

		Bool32 isValidForRender() const;

		// void renderBegin();
		void render(const RenderInfo &info, RenderResultInfo &resultInfo);
		// void renderEnd(const RenderInfo &info);

		const Color &getClearValueColor() const;
		void setClearValueColor(Color color);
		float getClearValueDepth() const;
		void setClearValueDepth(float value);
		uint32_t getClearValueStencil() const;
		void setClearValueStencil(uint32_t value);

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        const fd::CostTimer &getPreparingRenderCostTimer() const;
#endif //DEBUG and VG_ENABLE_COST_TIMER
	protected:
		const RenderTarget *m_pRenderTarget;

		Color m_clearValueColor;
		float m_clearValueDepth;
		uint32_t m_clearValueStencil;
		std::shared_ptr<vk::CommandPool> m_pCommandPool;
		std::shared_ptr<vk::CommandBuffer> m_pCommandBuffer;
		PipelineCache m_pipelineCache;

		uint32_t m_framebufferWidth;
		uint32_t m_framebufferHeight;

		//pre z pass
		Bool32 m_preZEnable;
		vk::Format m_preZDepthImageFormat;
		std::shared_ptr<Texture2DDepthAttachment> m_pPreZDepthAttachment;		
		std::shared_ptr<vk::RenderPass> m_pPreZRenderPass;
		std::shared_ptr<vk::Framebuffer> m_pPreZFramebuffer;
		CmdBuffer m_preZCmdBuffer;

		//post render
		Bool32 m_postRenderEnable;
		vk::Format m_postRenderColorImageFormat;
		vk::Format m_postRenderDepthStencilImageFormat;
		std::shared_ptr<Texture2DColorAttachment> m_pPostRenderColorAttachment;
		std::shared_ptr<TextureDepthStencilAttachment> m_pPostRenderDepthStencilAttachment;
		std::shared_ptr<vk::RenderPass> m_pPostRenderRenderPass;
		std::shared_ptr<vk::Framebuffer> m_pPostRenderFramebuffer;
		CmdBuffer m_postRenderCmdbuffer;
		
		CmdBuffer m_trunkRenderPassCmdBuffer;
		CmdBuffer m_trunkWaitBarrierCmdBuffer;
		CmdBuffer m_branchCmdBuffer;

		std::vector<BaseVisualObject *> m_bindedObjects;
		uint32_t m_bindedObjectCount;

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        fd::CostTimer m_preparingRenderCostTimer;
#endif //DEBUG and VG_ENABLE_COST_TIMER

		virtual void _preRender();
		virtual void _renderBegin(const RenderInfo & info, RenderResultInfo & resultInfo);
		virtual void _render(const RenderInfo & info, RenderResultInfo & resultInfo);
		virtual void _renderEnd(const RenderInfo & info, RenderResultInfo & resultInfo);
		virtual void _postRender();
		virtual Bool32 _isValidForRender() const;

		
		void _createCommandPool();
		void _createCommandBuffer();
		
		//void _createFence();

		void _recordCommandBufferForBegin();
		void _recordPreZRenderPassForBegin();
		void _recordPreZRenderPassForEnd();
		const vk::RenderPass * _recordTrunkRenderPassForBegin(Bool32 isPostRender, Bool32 isFirst);
		void _recordTrunkRenderPassForEnd();
		void _recordCommandBufferForEnd();

	    void _renderScene2(const Scene<SpaceType::SPACE_2> *pScene
		    , const Camera<SpaceType::SPACE_2> *pCamera
	        , const RenderInfo &info
			, CmdBuffer *pPreZCmdBuffer = nullptr
			, CmdBuffer *pBranchCmdBuffer = nullptr
            , CmdBuffer *pTrunkRenderPassCmdBuffer = nullptr
            , CmdBuffer *pTrunkWaitBarrierCmdBuffer = nullptr
			);

		void _renderScene3(const Scene<SpaceType::SPACE_3> *pScene
		    , const Camera<SpaceType::SPACE_3> *pCamera
	        , const RenderInfo &info
			, CmdBuffer *pPreZCmdBuffer = nullptr
			, CmdBuffer *pBranchCmdBuffer = nullptr
            , CmdBuffer *pTrunkRenderPassCmdBuffer = nullptr
            , CmdBuffer *pTrunkWaitBarrierCmdBuffer = nullptr
			);
	    
		void _beginBind();
		void _bind(BaseVisualObject *pVisublObject, BaseVisualObject::BindInfo & bindInfo, BaseVisualObject::BindResult *pResult);
		void _endBind();
        
		void _createPreZObjs();
		void _destroyPreZObjs();
		void _createPostRenderObjs();
		void _destroyPostRenderObjs();

		void _setBuildInData(BaseVisualObject * pVisualObject, Matrix4x4 modelMatrix, Matrix4x4 viewMatrix, Matrix4x4 projMatrix
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
	    , fd::CostTimer * pPreparingBuildInDataCostTimer
#endif //DEBUG and VG_ENABLE_COST_TIMER
		);
	private:
	};

	
} //namespace kgs

#endif // !VG_RENDERER_H
