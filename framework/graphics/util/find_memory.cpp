#include "graphics/util/find_memory.hpp"

namespace vg
{
	uint32_t findMemoryType(std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties)
	{
		vk::PhysicalDeviceMemoryProperties memProperties = pPhysicalDevice->getMemoryProperties();
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
		{
			if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw std::runtime_error("Failed to find suitable memory type!");

	}
}

