#ifndef GFW_VULKAN_EXT_H
#define GFW_VULKAN_EXT_H

#include <vulkan/vulkan.h>

namespace gfw
{
	VkResult createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);

	void destroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback,
		const VkAllocationCallbacks* pAllocator);
}

#endif // !GFW_VULKAN_EXT_H