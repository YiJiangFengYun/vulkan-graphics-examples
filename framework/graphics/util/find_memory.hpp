#ifndef VG_FIND_MEMORY_H
#define VG_FIND_MEMORY_H

#include <vulkan/vulkan.hpp>
namespace vg
{
	extern uint32_t findMemoryType(std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties);
} //namespace fd

#endif // !VG_FIND_MEMORY_H
