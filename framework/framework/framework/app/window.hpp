#ifndef VGF_WINDOW_H
#define VGF_WINDOW_H

#include <memory>
#include <vector>
#include <mutex>
#include "framework/global.hpp"

namespace vgf
{

	extern std::shared_ptr<GLFWwindow> createGLFWWindow(uint32_t width, uint32_t height, const char* title);

	extern std::shared_ptr<vk::SurfaceKHR> createSurface(vk::Instance *pInstance, GLFWwindow *pWindow);

	class Window
	{
	public:
		Window(uint32_t width
			, uint32_t height
			, const char* title
		);

		Window(std::shared_ptr<GLFWwindow> pWindow
			, std::shared_ptr<vk::SurfaceKHR> pSurface
		);

		virtual ~Window();
		void run();
		void Window::windowSetShouldClose(Bool32 value);
		vgf::Bool32 windowShouldClose();
		GLFWwindow *getGLFWWindow() const;
	protected:
		Window(const Window&) = delete;

		//--compositions
		uint32_t m_width;
		uint32_t m_height;
		std::shared_ptr<GLFWwindow> m_pWindow;
		std::shared_ptr<vk::SurfaceKHR> m_pSurface;
		std::shared_ptr<vk::SwapchainKHR> m_pSwapchain;
		std::vector<vk::Image> m_swapchainImages;
		vk::Format m_swapchainImageFormat;
		vk::Extent2D m_swapchainExtent;
		std::vector<std::shared_ptr<vk::ImageView>> m_pSwapchainImageViews;
		std::vector<vk::ImageView> m_swapchainImageViews;
		//vk::Queue m_presentQueue;
		//uint32_t m_presentQueueIndex;
		std::shared_ptr<vg::SurfaceRenderTarget> m_pRenderTarget;
		std::shared_ptr<vg::Renderer> m_pRenderer;
		std::shared_ptr<vk::Semaphore> m_pImageAvailableSemaphore;
		vk::PipelineStageFlags m_renderWaitStageMask;
		std::shared_ptr<vk::Semaphore> m_pRenderFinishedSemaphore;		
		int32_t m_currImageIndex;

		//std::mutex m_windowMutex;

		//temp memebers.
		std::shared_ptr<vk::CommandBuffer> m_pCurrCommandBuffer;

		//--aggregations

		virtual void _onResize() = 0;

		virtual void _onPreReCreateSwapchain() = 0;
		virtual void _onPostReCreateSwapchain() = 0;

		virtual void _onPreUpdate() = 0;
		virtual void _onUpdate() = 0;
		virtual void _onPostUpdate() = 0;

		virtual void _onPreDraw() = 0;
		virtual void _onDraw() = 0;
		virtual void _onPostDraw() = 0;

        virtual void _onPreRender(vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo) = 0;
		virtual void _onRender(vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo) = 0;
		virtual void _onPostRender(vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo) = 0;

		virtual void _doUpdate();
		virtual void _doDraw();
		virtual void _doRender(vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo);

		// tool methods
		void _createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling,
			vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties,
			std::shared_ptr<vk::Image>& pImage, std::shared_ptr<vk::DeviceMemory>& pImageMemory);
		std::shared_ptr<vk::ImageView> _createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);
		uint32_t _findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);

	private:
	    void _createWindow(uint32_t width, uint32_t height, const char* title);
		void _createSurface();
		void _createSwapchain();
		void _createSwapchainImageViews();
		void _createRenderer();
		void _createSemaphores();

#ifdef USE_IMGUI_BIND
		Bool32 m_mousePressed[3];
        void _initIMGUI();
#endif //USE_IMGUI_BIND

		void _doReCreateSwapchain();
		void _reCreateSwapchain();

		//event handlers
		void _onWindowResized(int32_t width, int32_t height);

		friend void onWindowResized(GLFWwindow *window, int32_t width, int32_t height);
		
	};
}  //namespace gfw

#endif // !VGF_WINDOW_H

