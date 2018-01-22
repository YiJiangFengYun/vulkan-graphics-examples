#include "graphics/util/swapchain_info.hpp"

namespace vg
{
	SwapChainSupportDetails SwapChainSupportDetails::querySwapChainSupport(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface) {
		SwapChainSupportDetails details;
		details.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
		details.formats = physicalDevice.getSurfaceFormatsKHR(surface);
		details.presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
		return details;
	}

	vk::SurfaceFormatKHR SwapChainSupportDetails::chooseSurfaceFormat()
	{
		if (this->formats.size() == 1 && this->formats[0].format == vk::Format::eUndefined)
		{
			return { vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear };
		}

		for (const auto& format : this->formats)
		{
			if (format.format == vk::Format::eB8G8R8A8Unorm &&
				format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
			{
				return format;
			}
		}

		return this->formats[0];
	}

	vk::PresentModeKHR SwapChainSupportDetails::choosePresentMode()
	{
		std::unordered_map<vk::PresentModeKHR, uint32_t> presentModeAndCores = {
			{ vk::PresentModeKHR::eMailbox, 0 },
			{ vk::PresentModeKHR::eFifo, 1 },
			{ vk::PresentModeKHR::eImmediate, 2 }
		};

		vk::PresentModeKHR currPresentMode = vk::PresentModeKHR::eImmediate;
		uint32_t currCore = presentModeAndCores[currPresentMode];
		for (const auto& presentMode : this->presentModes)
		{
			auto iterator = presentModeAndCores.find(presentMode);

			if (iterator != presentModeAndCores.end() && iterator->second < currCore)
			{
				currPresentMode = iterator->first;
				currCore = iterator->second;
			}
		}

		return currPresentMode;
	}

	vk::Extent2D SwapChainSupportDetails::chooseExtent(int width, int height)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()
			&& capabilities.currentExtent.height != 0u)
		{
			return capabilities.currentExtent;
		}
		else
		{
			VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

			actualExtent.width = std::max(capabilities.minImageExtent.width,
				std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height,
				std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}
} //namespace kgs