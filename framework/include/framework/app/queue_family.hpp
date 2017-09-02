#ifndef GFW_FAMILIES_H
#define GFW_FAMILIES_H

#include <vulkan/vulkan.hpp>
#include "framework/global.hpp"

namespace gfw
{
	struct UsedQueueFamily
	{
		int32_t graphicsFamily = -1;
		int32_t presentFamily = -1;

		Bool32 isComplete()
		{
			return graphicsFamily >= 0 && presentFamily >= 0;
		}

		UsedQueueFamily static findQueueFamilies(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface);
	};
}

#endif // !GFW_FAMILIES_H

