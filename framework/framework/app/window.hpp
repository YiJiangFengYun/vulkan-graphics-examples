#ifndef VGF_WINDOW_H
#define VGF_WINDOW_H

#include <memory>
#include <vector>
#include <mutex>
#include <foundation/foundation.hpp>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <graphics/graphics.hpp>
#include "framework/global.hpp"

namespace vgf
{

	extern std::shared_ptr<GLFWwindow> createGLFWWindow(uint32_t width, uint32_t height, const char* title);

	extern std::shared_ptr<vk::SurfaceKHR> createSurface(std::shared_ptr<vk::Instance> pInstance,
		std::shared_ptr<GLFWwindow> pWindow);

	class Window
	{
	public:
		enum class RenderType
		{
			RENDERER_2,
			RENDERER_3,
			BEGIN_RANGE = RENDERER_2,
			END_RANGE = RENDERER_3,
			RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
		};

		template<RenderType type>
		struct RenderTypeInfo
		{
			typedef vg::BaseRenderer RendererType;
		};

		template<>
		struct RenderTypeInfo<RenderType::RENDERER_2>
		{
			typedef vg::Renderer2 RendererType;
		};

		template<>
		struct RenderTypeInfo<RenderType::RENDERER_3>
		{
			typedef vg::Renderer3 RendererType;
		};

		Window(uint32_t width
			, uint32_t height
			, const char* title
		);

		Window(uint32_t width
			, uint32_t height
			, const char* title
			, RenderType renderType
		);

		Window(std::shared_ptr<GLFWwindow> pWindow
			, std::shared_ptr<vk::SurfaceKHR> pSurface
		);

		Window(RenderType renderType
			, std::shared_ptr<GLFWwindow> pWindow
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
		RenderType m_renderType;

		std::shared_ptr<GLFWwindow> m_pWindow;
		std::shared_ptr<vk::SurfaceKHR> m_pSurface;
		std::shared_ptr<vk::SwapchainKHR> m_pSwapchain;
		std::vector<vk::Image> m_swapchainImages;
		vk::Format m_swapchainImageFormat;
		vk::Extent2D m_swapchainExtent;
		std::vector<std::shared_ptr<vk::ImageView>> m_pSwapchainImageViews;
		//vk::Queue m_presentQueue;
		//uint32_t m_presentQueueIndex;
		std::vector<std::shared_ptr<vg::BaseRenderer>> m_pRenderers;
		std::shared_ptr<vk::Semaphore> m_pImageAvailableSemaphore;

		//std::mutex m_windowMutex;

		//temp memebers.
		std::shared_ptr<vk::CommandBuffer> m_pCurrCommandBuffer;

		//--aggregations

		void _createWindow(uint32_t width, uint32_t height, const char* title);
		void _createSurface();
		//void _allocatePresentQueue();
		void _createSwapchain();
		void _createSwapchainImageViews();
		void _createRenderers();
		void _createSemaphores();

		//void _freePresentQueue();


		void _doUpdate();
		void _doRender();

		void _doReCreateSwapchain();

		virtual void _onPreReCreateSwapchain() = 0;
		virtual void _reCreateSwapchain();
		virtual void _onPostReCreateSwapchain() = 0;

		virtual void _onPreUpdate() = 0;
		virtual void _update() = 0;
		virtual void _onPostUpdate() = 0;

		virtual void _onPreRender() = 0;
		virtual void _render();
		virtual void _onPostRender() = 0;

		// tool methods
		void _createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling,
			vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties,
			std::shared_ptr<vk::Image>& pImage, std::shared_ptr<vk::DeviceMemory>& pImageMemory);
		std::shared_ptr<vk::ImageView> _createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);
		uint32_t _findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);

		//event handlers
		void _onWindowResized(int32_t width, int32_t height);

		friend void onWindowResized(GLFWwindow *window, int32_t width, int32_t height);
	};
}  //namespace gfw

#include "framework/app/window.inl"


#endif // !VGF_WINDOW_H

