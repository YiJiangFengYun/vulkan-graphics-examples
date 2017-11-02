#ifndef KGS_VULKAN_EXT_H
#define KGS_VULKAN_EXT_H

#include <vulkan/vulkan.h>

namespace kgs
{
	VkResult createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);

	void destroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback,
		const VkAllocationCallbacks* pAllocator);
}

#endif // !KGS_VULKAN_EXT_H