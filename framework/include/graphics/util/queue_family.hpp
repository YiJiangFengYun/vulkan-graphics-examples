#ifndef KGS_QUEUE_FAMILY_H
#define KGS_QUEUE_FAMILY_H

#include "graphics/global.hpp"

namespace kgs
{
	struct UsedQueueFamily
	{
		int32_t graphicsFamily;
		int32_t presentFamily;

		UsedQueueFamily();

		Bool32 isComplete() const;

		UsedQueueFamily static findQueueFamilies(const vk::PhysicalDevice& physicalDevice
			, const vk::SurfaceKHR& surface);
	};
}

#endif // !KGS_QUEUE_FAMILY_H

