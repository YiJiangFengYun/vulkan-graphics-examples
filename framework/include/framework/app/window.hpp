#ifndef GFW_CONTEXT_H
#define GFW_CONTEXT_H

#include <memory>
#include <vector>
#include <mutex>
#include <vulkan/vulkan.hpp>
#include "framework/global.hpp"
#include "framework/app/vulkan_ext.hpp"
#include <GLFW/glfw3.h>
#include "framework/app/queue_family.hpp"
#include "foundation/wrapper.hpp"
#include <graphics/renderer/renderer.hpp>

namespace gfw
{
	struct SwapChainSupportDetails {
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
		SwapChainSupportDetails static querySwapChainSupport(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface);
	    vk::SurfaceFormatKHR chooseSurfaceFormat();
		vk::PresentModeKHR choosePresentMode();
		vk::Extent2D chooseExtent(GLFWwindow* window);
	};

	class Window
	{
	public:
		Window(uint32_t width, uint32_t height, const char* title,
			std::shared_ptr<vk::Instance> pInstance, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
			std::shared_ptr<vk::Device> device, vk::Queue graphicsQueue, vk::Queue presentQueue);
		Window(std::shared_ptr<GLFWwindow> pWindow, std::shared_ptr<vk::SurfaceKHR> pSurface,
			std::shared_ptr<vk::Instance> pInstance, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
			std::shared_ptr<vk::Device> device, vk::Queue graphicsQueue, vk::Queue presentQueue);
		virtual ~Window();
		void run();
		void Window::windowSetShouldClose(Bool32 value);
		gfw::Bool32 windowShouldClose();
		std::shared_ptr<GLFWwindow> getGLFWWindow() const;
	private:
		Window(const Window&);

		//--compositions
		std::shared_ptr<GLFWwindow> m_pWindow;
		std::shared_ptr<vk::SurfaceKHR> m_pSurface;
		std::shared_ptr<vk::SwapchainKHR> m_pSwapchain;
		std::vector<vk::Image> m_swapchainImages;
		vk::Format m_swapchainImageFormat;
		vk::Extent2D m_swapchainExtent;
		std::vector<std::shared_ptr<vk::ImageView>> m_pSwapchainImageViews;
		std::shared_ptr<vk::CommandPool> m_pCommandPool;
		std::vector<std::shared_ptr<kgs::BaseRenderer>> m_pRenderers;
		std::shared_ptr<vk::Semaphore> m_pImageAvailableSemaphore;
		std::shared_ptr<vk::Semaphore> m_pRenderFinishedSemaphore;

		//std::mutex m_windowMutex;

		//temp memebers.
		std::shared_ptr<vk::CommandBuffer> m_pCurrCommandBuffer;

		//--aggregations
		std::shared_ptr<vk::Instance> m_pVKInstance;
		std::shared_ptr<vk::PhysicalDevice> m_pPhysicalDevice;
		std::shared_ptr<vk::Device> m_pDevice;
		vk::Queue m_graphicsQueue;
		vk::Queue m_presentQueue;

		void _createWindow(uint32_t width, uint32_t height, const char* title);
		void _createSurface();
		void _createSwapchain();
		void _createSwapchainImageViews();
		void _createCommandPool();
		void _createRenderers();
		void _createSemaphores();

		void _update();
		void _render();

		std::shared_ptr<vk::CommandBuffer> _beginRender();
		void _endRender();

		// tool methods
		void _createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling,
			vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties,
			std::shared_ptr<vk::Image>& pImage, std::shared_ptr<vk::DeviceMemory>& pImageMemory);
		std::shared_ptr<vk::ImageView> _createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);
		void _transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
		uint32_t _findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
		std::shared_ptr<vk::CommandBuffer> _beginSingleTimeCommands();
		void _endSingleTimeCommands(std::shared_ptr<vk::CommandBuffer> pCommandBuffer);

		//event handlers
		void _onWindowResized(int32_t width, int32_t height);

		friend void onWindowResized(GLFWwindow *window, int32_t width, int32_t height);
	};
}



#endif // !GFW_CONTEXT_H

