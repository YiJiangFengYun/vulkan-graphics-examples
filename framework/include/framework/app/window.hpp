#ifndef GFW_CONTEXT_H
#define GFW_CONTEXT_H

#include <vector>
#include <vulkan/vulkan.hpp>
#include "macro.hpp"
#include "vulkan_ext.hpp"
#include <GLFW/glfw3.h>
#include "queue_family.hpp"

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
			vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device,
			vk::Queue graphicsQueue, vk::Queue presentQueue);
		Window(GLFWwindow *pWindow, vk::SurfaceKHR surface,
			vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device,
			vk::Queue graphicsQueue, vk::Queue presentQueue);
		~Window();
		void update();
		vk::CommandBuffer beginRender();
		void endRender();
		gfw::Bool32 windowShouldClose();
		GLFWwindow *getGLFWWindow() const;
	private:
		Window(const Window&);

		//static
		static vk::Format DEFAULT_DEPTH_FORMAT;

		//compositions
		GLFWwindow *m_pWindow;
		vk::SurfaceKHR m_surface;
		vk::SwapchainKHR m_swapchain;
		std::vector<vk::Image> m_swapchainImages;
		vk::Format m_swapchainImageFormat;
		vk::Extent2D m_swapchainExtent;
		std::vector<vk::ImageView> m_swapchainImageViews;
		vk::CommandPool m_commandPool;
		vk::RenderPass m_renderPass;
		vk::Image m_depthImage;
		vk::ImageView m_depthImageView;
		vk::DeviceMemory m_depthImageMemory;
		vk::Format m_depthFormat;
		std::vector<vk::Framebuffer> m_swapchainFramebuffers;
		vk::Semaphore m_imageAvailableSemaphore;
		vk::Semaphore m_renderFinishedSemaphore;

		//temp memebers.
		vk::CommandBuffer m_currCommandBuffer;

		//aggregations
		vk::Instance m_vkInstance;
		vk::PhysicalDevice m_physicalDevice;
		vk::Device m_device;
		vk::Queue m_graphicsQueue;
		vk::Queue m_presentQueue;

		void _createWindow(uint32_t width, uint32_t height, const char* title);
		void _destroyWindow();
		void _createSurface();
		void _destroySurface();
		void _createSwapchain();
		void _destroySwapchain();
		void _createSwapchainImageViews();
		void _destroySwapchainImageViews();
		void _createCommandPool();
		void _destroyCommandPool();
		void _createRenderPass();
		void _destroyRenderPass();
		void _checkDepthFormat();
		void _createDepthResources();
		void _destroyDepthResources();
		void _createFramebuffers();
		void _destroyFramebuffers();
		void _createSemaphores();
		void _destroySemaphores();
		vk::CommandBuffer _beginRender();
		void _endRender();

		// tool methods
		void _createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling,
			vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image& image, vk::DeviceMemory& imageMemory);
		void _createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags, vk::ImageView& imageView);
		void _transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
		uint32_t _findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
		vk::CommandBuffer _beginSingleTimeCommands();
		void _endSingleTimeCommands(vk::CommandBuffer commandBuffer);

		//event handlers
		void _onWindowResized(int32_t width, int32_t height);

		friend void onWindowResized(GLFWwindow *window, int32_t width, int32_t height);
	};
}



#endif // !GFW_CONTEXT_H

