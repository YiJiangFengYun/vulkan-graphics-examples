#ifndef KGS_SWAPCHAIN_INFO_H
#define KGS_SWAPCHAIN_INFO_H

#include <unordered_map>
#include "graphics/global.hpp"

namespace kgs
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
} //namespace kgs

#endif // !KGS_SWAPCHAIN_INFO_H
