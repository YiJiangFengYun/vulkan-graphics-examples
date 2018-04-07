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
			const SceneAndCamera *pSceneAndCamera;
			uint32_t sceneAndCameraCount;
			uint32_t waitSemaphoreCount;
			const vk::Semaphore* pWaitSemaphores;
		};

		struct RenderResultInfo {
			Bool32 isRendered;
			uint32_t signalSemaphoreCount;
			const vk::Semaphore* pSignalSemaphores;
			uint32_t drawCount;
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
		//std::shared_ptr<vk::Fence> m_waitFence;
		std::shared_ptr<vk::Semaphore> m_cachePSemaphore;
		std::vector<vk::Semaphore> m_arrSemaphores;

		CmdBuffer m_trunkRenderPassCmdBuffer;
		CmdBuffer m_trunkWaitBarrierCmdBuffer;
		CmdBuffer m_branchCmdBuffer;

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
		void _createSemaphore();
		//void _createFence();

		void _recordCommandBufferForBegin();
		void _recordTrunkWaitBarrier(CmdBuffer *pTrunkRenderPassCmdBuffer);
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

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)

#endif //DEBUG and VG_ENABLE_COST_TIMER

	private:
	};

	
} //namespace kgs

#endif // !VG_RENDERER_H
