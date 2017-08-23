#ifndef FW_CONTEXT_H
#define FW_CONTEXT_H

#include <vector>
#include <vulkan/vulkan.hpp>
#include "macro.hpp"
#include "vulkan_ext.hpp"
#include <GLFW/glfw3.h>
#include "families.hpp"

namespace fw
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

	class Surface
	{
	public:
		Surface(uint32_t width, uint32_t height, const char* title, 
			vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device);
		Surface(GLFWwindow *pWindow, vk::SurfaceKHR surface,
			vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device);
		~Surface();
		GLFWwindow *getWindow() const;
	private:
		//compositions
		GLFWwindow *m_pWindow;
		vk::SurfaceKHR m_surface;
		vk::SwapchainKHR m_swapchain;
		std::vector<vk::Image> m_swapchainImages;
		vk::Format m_swapchainImageFormat;
		vk::Extent2D m_swapchainExtent;
		std::vector<vk::ImageView> m_swapchainImageViews;

		//aggregations
		vk::Instance m_vkInstance;
		vk::PhysicalDevice m_physicalDevice;
		vk::Device m_device;

		void _createWindow(uint32_t width, uint32_t height, const char* title);
		void _createSurface();
		void _createSwapchain();
		void _createSwapchainImageViews();
		// tool methods
		vk::ImageView _createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);


		void _onWindowResized(int32_t width, int32_t height);

		friend void onWindowResized(GLFWwindow *window, int32_t width, int32_t height);
	};
}



#endif // !FW_CONTEXT_H

