#ifndef VG_RENDERER_H
#define VG_RENDERER_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/scene.hpp"
#include "graphics/scene/camera.hpp"
#include "graphics/renderer/renderer_option.hpp"
#include "graphics/texture/texture_color_attachment.hpp"
#include "graphics/texture/texture_depth_stencil_attachment.hpp"

//todo: batch mesh,
//todo: cache graphics pipeline.

namespace vg
{
	class BaseRenderer : public Base
	{
	public:
		static const vk::Format DEFAULT_DEPTH_STENCIL_FORMAT;

		struct RenderInfo {
			uint32_t                         waitSemaphoreCount;
			const vk::Semaphore*             pWaitSemaphores;
		};

		struct RenderResultInfo {
			uint32_t                         signalSemaphoreCount;
			const vk::Semaphore*             pSignalSemaphores;
		};

		BaseRenderer(std::shared_ptr<vk::ImageView> pSwapchainImageView
			, vk::Format swapchainImageFormat
			, uint32_t swapchainImageWidth
			, uint32_t swapchainImageHeight
		);

		BaseRenderer(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
		);

		~BaseRenderer();

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

		//Grahics queue for render safe in multiply threads.
		//vk::Queue m_graphicsQueue;
		//uint32_t m_graphicsQueueIndex;

		std::vector<std::shared_ptr<vk::PipelineLayout>> m_arrPLastPipelineLayouts;
		std::vector<std::shared_ptr<vk::Pipeline>> m_arrPLastPipelines;
		std::vector<std::shared_ptr<vk::Semaphore>> m_arrPLastSemaphores;
		std::vector<vk::Semaphore> m_arrSemaphores;
		//aggregations
		//Renderer will use swapchain image when color attachment texture is null.
		std::shared_ptr<vk::ImageView> m_pSwapchainImageView;
		vk::Format m_swapchainImageFormat;
		//Renderer will render to color texture when it is not null.
		std::shared_ptr<TextureColorAttachment> m_pColorTexture;

		virtual void _render(const RenderInfo &info, RenderResultInfo &resultInfo);
		virtual Bool32 _isValidForRender();

		void _createRenderPass();
		void _createDepthStencilTex();
		void _createFramebuffer();
		void _createCommandPool();
		void _createCommandBuffer();

		void _createPipelineForRender(std::shared_ptr<vk::PipelineLayout> &pPipelineLayout, 
			std::shared_ptr<vk::Pipeline> &pPipeline,
			std::shared_ptr<BaseMesh> pMesh,
			std::shared_ptr<Material> pMaterial,
			uint32_t subMeshIndex = 0u,
			uint32_t passIndex = 0u);
		void _recordCommandBufferForRender(std::shared_ptr<vk::PipelineLayout> pPipelineLayout,
			std::shared_ptr<vk::Pipeline> pPipeline,
			std::shared_ptr<BaseMesh> pMesh,
			std::shared_ptr<Material> pMaterial,
			uint32_t subMeshIndex = 0u,
			uint32_t passIndex = 0u);

	private:
		BaseRenderer() = delete;
		inline void _init();
	};

	template <SpaceType SPACE_TYPE>
	class Renderer : public BaseRenderer
	{
	public:
		typedef Scene<SPACE_TYPE> SceneType;
		typedef Camera<SPACE_TYPE> CameraType;

		Renderer(std::shared_ptr<vk::ImageView> pSwapchainImageView
			, vk::Format swapchainImageFormat
			, uint32_t swapchainImageWidth
			, uint32_t swapchainImageHeight
		);

		Renderer(std::shared_ptr<vk::ImageView> pSwapchainImageView
			, vk::Format swapchainImageFormat
			, uint32_t swapchainImageWidth
			, uint32_t swapchainImageHeight
			, std::shared_ptr<SceneType> pScene
			, std::shared_ptr<CameraType> pCamera
		);

		Renderer(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
		);

		Renderer(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
			, std::shared_ptr<SceneType> pScene
			, std::shared_ptr<CameraType> pCamera
		);

		void reset(std::shared_ptr<SceneType> pScene
			, std::shared_ptr<CameraType> pCamera
		);

	protected:
		//compositions

		//aggregations
		std::shared_ptr<SceneType> m_pScene;
		std::shared_ptr<CameraType> m_pCamera;

		Bool32 _isValidForRender() override;

		void _render(const RenderInfo &info, RenderResultInfo &resultInfo) override;

		inline virtual typename SpaceTypeInfo<SPACE_TYPE>::MatrixType _getMVPMatrix(std::shared_ptr<typename SceneType::ObjectType> pObject);

		inline virtual typename SpaceTypeInfo<SPACE_TYPE>::MatrixType _getMVMatrix(std::shared_ptr<typename SceneType::ObjectType> pObject);

	private:
		Renderer() = delete;
	};
} //namespace kgs

#include "graphics/renderer/renderer.inl"

#endif // !VG_RENDERER_H
