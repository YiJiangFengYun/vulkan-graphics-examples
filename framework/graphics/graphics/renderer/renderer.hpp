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

//todo: batch mesh,
//todo: cache graphics pipeline.

namespace vg
{
	
	class Renderer : public Base
	{
	public:
		static const vk::Format DEFAULT_DEPTH_STENCIL_FORMAT;

		struct SceneAndCamera {
			const BaseScene *pScene;
			const BaseCamera *pCamera;
		};

		struct RenderInfo {
			const SceneAndCamera *pSceneAndCameras;
			uint32_t sceneAndCameraCount;
			uint32_t waitSemaphoreCount;
			const vk::Semaphore* pWaitSemaphores;
			const vk::PipelineStageFlags* pWaitDstStageMask;
			uint32_t signalSemaphoreCount;
			const vk::Semaphore* pSignalSemaphores;

			RenderInfo(uint32_t sceneAndCameraCount = 0u
				, const SceneAndCamera *pSceneAndCameras = nullptr
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

		Renderer();

		~Renderer();

		Bool32 isValidForRender() const;

		// void renderBegin();
		void render(const RenderInfo &info, RenderResultInfo &resultInfo);
		// void renderEnd(const RenderInfo &info);

		uint32_t getFramebufferWidth() const ;
		uint32_t getFramebufferHeight() const;
		vk::Format getColorImageFormat() const;
		vk::Format getDepthStencilImageFormat() const;

		const Color &getClearValueColor() const;
		void setClearValueColor(Color color);
		float getClearValueDepth() const;
		void setClearValueDepth(float value);
		uint32_t getClearValueStencil() const;
		void setClearValueStencil(uint32_t value);
		const fd::Rect2D &getClearArea() const;
		void setClearArea(fd::Rect2D area);

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        const fd::CostTimer &getPreparingRenderCostTimer() const;
#endif //DEBUG and VG_ENABLE_COST_TIMER
	protected:
		//compositions
		uint32_t m_framebufferWidth;
		uint32_t m_framebufferHeight;
		vk::Format m_colorImageFormat;
		vk::Format m_depthStencilImageFormat;
		Color m_clearValueColor;
		float m_clearValueDepth;
		uint32_t m_clearValueStencil;
		fd::Rect2D m_renderArea;
		vk::RenderPass *m_pCurrRenderPass;
		vk::Framebuffer* m_pCurrFrameBuffer;
		std::shared_ptr<vk::CommandPool> m_pCommandPool;
		std::shared_ptr<vk::CommandBuffer> m_pCommandBuffer;
		PipelineCache m_pipelineCache;
		
		CmdBuffer m_trunkRenderPassCmdBuffer;
		CmdBuffer m_trunkWaitBarrierCmdBuffer;
		CmdBuffer m_branchCmdBuffer;

		std::vector<BaseVisualObject *> m_bindedObjects;
		uint32_t m_bindedObjectCount;

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        fd::CostTimer m_preparingRenderCostTimer;
#endif //DEBUG and VG_ENABLE_COST_TIMER

		virtual void _preRender();
		virtual void _renderBegin();
		virtual void _render(const RenderInfo &info, RenderResultInfo &resultInfo);
		virtual void _renderEnd(const RenderInfo &info);
		virtual void _postRender();
		virtual Bool32 _isValidForRender() const;

		
		void _createCommandPool();
		void _createCommandBuffer();
		
		//void _createFence();

		void _recordCommandBufferForBegin();
		void _recordTrunkRenderPassForBegin();

		void _recordTrunkRenderPassForEnd();
		void _recordCommandBufferForEnd();

	    void _renderScene2(const Scene<SpaceType::SPACE_2> *pScene
		    , const Camera<SpaceType::SPACE_2> *pCamera
	        , const RenderInfo &info
			, CmdBuffer *pBranchCmdBuffer = nullptr
            , CmdBuffer *pTrunkRenderPassCmdBuffer = nullptr
            , CmdBuffer *pTrunkWaitBarrierCmdBuffer = nullptr
			);

		void _renderScene3(const Scene<SpaceType::SPACE_3> *pScene
		    , const Camera<SpaceType::SPACE_3> *pCamera
	        , const RenderInfo &info
			, CmdBuffer *pBranchCmdBuffer = nullptr
            , CmdBuffer *pTrunkRenderPassCmdBuffer = nullptr
            , CmdBuffer *pTrunkWaitBarrierCmdBuffer = nullptr
			);
	    
		void _beginBind();
		void _bind(BaseVisualObject *pVisublObject, BaseVisualObject::BindInfo & bindInfo, BaseVisualObject::BindResult *pResult);
		void _endBind();
        

	private:
	};

	
} //namespace kgs

#endif // !VG_RENDERER_H
