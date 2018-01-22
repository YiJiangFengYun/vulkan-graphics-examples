#ifndef VG_QUEUE_FAMILY_H
#define VG_QUEUE_FAMILY_H

#include "graphics/global.hpp"

namespace vg
{
	struct UsedQueueFamily
	{
		int32_t graphicsFamily;
		int32_t presentFamily;
		uint32_t graphicsMaxQueueCount;
		uint32_t presentMaxQueueCount;

		UsedQueueFamily();

		Bool32 isComplete() const;

		UsedQueueFamily static findQueueFamilies(const vk::PhysicalDevice& physicalDevice
			, const vk::SurfaceKHR& surface);
	};
}

#endif // !VG_QUEUE_FAMILY_H

