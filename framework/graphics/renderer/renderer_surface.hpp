#ifndef VG_RENDERER_SURFACE_HPP
#define VG_RENDERER_SURFACE_HPP

#include "graphics/renderer/renderer.hpp"

namespace vg
{
    class SurfaceRenderer : public Renderer 
	{
	public:
	    SurfaceRenderer(uint32_t swapchainImageViewCount
			, vk::ImageView *pSwapchainImageViews
			, vk::Format swapchainImageFormat
			, uint32_t swapchainImageWidth
			, uint32_t swapchainImageHeight);
		void setImageIndex(uint32_t imageIndex);
	private:
	    std::shared_ptr<vk::RenderPass> m_pRenderPass;
		std::shared_ptr<BaseDepthStencilAttachment> m_pDepthStencilAttachment;
		std::vector<std::shared_ptr<vk::Framebuffer>> m_pFrameBuffers;
		//Renderer will use swapchain image when color attachment texture is null.
		uint32_t m_swapchainImageViewCount;
		vk::ImageView *m_pSwapchainImageViews;
        uint32_t m_imageIndex;

		std::shared_ptr<vk::Semaphore> m_cachePSemaphore;

		void _createRenderPass();
		void _createDepthStencilTex();
		void _createFramebuffers();
		void _createSemaphore();
	};
} //vg

#endif //VG_RENDERER_SURFACE_HPP