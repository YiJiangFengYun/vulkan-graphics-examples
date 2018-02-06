#ifndef VG_RENDERER_H
#define VG_RENDERER_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/scene.hpp"
#include "graphics/scene/camera.hpp"
#include "graphics/renderer/renderer_option.hpp"
#include "graphics/texture/texture_color_attachment.hpp"
#include "graphics/texture/texture_depth_stencil_attachment.hpp"
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
			uint32_t countSceneAndCamera;
			uint32_t waitSemaphoreCount;
			const vk::Semaphore* pWaitSemaphores;
		};

		struct RenderResultInfo {
			Bool32 isRendered;
			uint32_t                         signalSemaphoreCount;
			const vk::Semaphore*             pSignalSemaphores;
		};

		Renderer(std::shared_ptr<vk::ImageView> pSwapchainImageView
			, vk::Format swapchainImageFormat
			, uint32_t swapchainImageWidth
			, uint32_t swapchainImageHeight
		);

		Renderer(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
		);

		~Renderer();

		Bool32 isValidForRender();

		void render(const RenderInfo &info, RenderResultInfo &resultInfo);

		uint32_t getFramebufferWidth();
		uint32_t getFramebufferHeight();
		vk::Format getColorImageFormat();
		vk::Format getDepthStencilImageFormat();

		const Color &getClearValueColor() const;
		void setClearValueColor(Color color);
		float getClearValueDepth() const;
		void setClearValueDepth(float value);
		uint32_t getClearValueStencil() const;
		void setClearValueStencil(uint32_t value);
		const fd::Rect2D &getClearArea() const;
		void setClearArea(fd::Rect2D area);
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
		std::shared_ptr<vk::RenderPass> m_pRenderPass;
		std::shared_ptr<TextureDepthStencilAttachment> m_pDepthStencilTexture;
		std::shared_ptr<vk::Framebuffer> m_pFrameBuffer;
		//Command pool for render safe in multiply threads.
		std::shared_ptr<vk::CommandPool> m_pCommandPool;
		std::shared_ptr<vk::CommandBuffer> m_pCommandBuffer;
		PipelineCache m_pipelineCache;
		
		std::vector<std::shared_ptr<vk::Semaphore>> m_arrCachePSemaphores;
		std::vector<vk::Semaphore> m_arrSemaphores;
		//aggregations
		//Renderer will use swapchain image when color attachment texture is null.
		std::shared_ptr<vk::ImageView> m_pSwapchainImageView;
		vk::Format m_swapchainImageFormat;
		//Renderer will render to color texture when it is not null.
		std::shared_ptr<TextureColorAttachment> m_pColorTexture;

		virtual void _preRender();
		virtual void _render(const RenderInfo &info, RenderResultInfo &resultInfo);
		virtual void _postRender();
		virtual Bool32 _isValidForRender();

		void _createRenderPass();
		void _createDepthStencilTex();
		void _createFramebuffer();
		void _createCommandPool();
		void _createCommandBuffer();

		void _createPipelineForRender(std::shared_ptr<vk::Pipeline> &pPipeline,
			std::shared_ptr<BaseMesh> pMesh,
			std::shared_ptr<Material> pMaterial,
			uint32_t subMeshIndex = 0u,
			uint32_t passIndex = 0u);
		void _recordCommandBufferForRender(std::shared_ptr<vk::Pipeline> pPipeline,
			std::shared_ptr<BaseMesh> pMesh,
			std::shared_ptr<Material> pMaterial,
			uint32_t subMeshIndex = 0u,
			uint32_t passIndex = 0u);

	    void _renderScene2(const Scene<SpaceType::SPACE_2> *pScene
		    , const Camera<SpaceType::SPACE_2> *pCamera
	        , const RenderInfo &info
	    	, RenderResultInfo &resultInfo);

		 void _renderScene3(const Scene<SpaceType::SPACE_3> *pScene
		    , const Camera<SpaceType::SPACE_3> *pCamera
	        , const RenderInfo &info
	    	, RenderResultInfo &resultInfo);

	private:
		Renderer() = delete;
		void _init();
	};
} //namespace kgs

#endif // !VG_RENDERER_H
