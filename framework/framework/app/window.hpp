#ifndef VGF_WINDOW_H
#define VGF_WINDOW_H

#include <memory>
#include <vector>
#include <mutex>
#include "framework/global.hpp"

namespace vgf
{

	extern std::shared_ptr<GLFWwindow> createGLFWWindow(uint32_t width, uint32_t height, const char* title);

	extern std::shared_ptr<vk::SurfaceKHR> createSurface(std::shared_ptr<vk::Instance> pInstance,
		std::shared_ptr<GLFWwindow> pWindow);

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
		std::shared_ptr<GLFWwindow> getGLFWWindow() const;
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
		//vk::Queue m_presentQueue;
		//uint32_t m_presentQueueIndex;
		std::vector<std::shared_ptr<vg::Renderer>> m_pRenderers;
		std::shared_ptr<vk::Semaphore> m_pImageAvailableSemaphore;
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

		virtual void _onPreRender() = 0;
		virtual void _onRender() = 0;
		virtual void _onPostRender() = 0;

		virtual std::shared_ptr<vg::Renderer> _createRenderer(std::shared_ptr<vk::ImageView> pSwapchainImageView
		    , vk::Format swapchainImageFormat
			, uint32_t swapchainImageWidth
			, uint32_t swapchainImageHeight
		);

		virtual void _renderWithRenderer(const std::shared_ptr<vg::Renderer> &pRenderer
		    , const vg::Renderer::RenderInfo &info
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
		void _createRenderers();
		void _createSemaphores();

#ifdef USE_IMGUI_BIND
		Bool32 m_mousePressed[3];
        void _initIMGUI();
#endif //USE_IMGUI_BIND


		void _doUpdate();
		void _doRender();

		void _doReCreateSwapchain();
		void _reCreateSwapchain();

		//event handlers
		void _onWindowResized(int32_t width, int32_t height);

		friend void onWindowResized(GLFWwindow *window, int32_t width, int32_t height);
		
	};
}  //namespace gfw

#include "framework/app/window.inl"


#endif // !VGF_WINDOW_H

