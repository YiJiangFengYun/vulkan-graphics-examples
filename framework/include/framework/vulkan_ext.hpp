#ifndef FW_VULKAN_EXT_H
#define FW_VULKAN_EXT_H

#include <vulkan/vulkan.h>

namespace fw
{
	VkResult createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);

	void destroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback,
		const VkAllocationCallbacks* pAllocator);
}

#endif // !FW_VULKAN_EXT_H