#ifndef KGS_FIND_MEMORY_H
#define KGS_FIND_MEMORY_H

#include <vulkan/vulkan.hpp>
namespace kgs
{
	extern uint32_t _findMemoryType(std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties);
} //namespace fd

#endif // !KGS_FIND_MEMORY_H
