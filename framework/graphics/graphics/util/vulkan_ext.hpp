#ifndef VG_VULKAN_EXT_H
#define VG_VULKAN_EXT_H

#include <vulkan/vulkan.h>

namespace vg
{
    VkResult createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);

    void destroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback,
        const VkAllocationCallbacks* pAllocator);
}

#endif // !VG_VULKAN_EXT_H