#ifndef KGS_DEVICE_H
#define KGS_DEVICE_H

#include <vulkan/vulkan.hpp>

namespace kgs
{
	class Device
	{
	public:
		Device(vk::PhysicalDevice physicalDevice, vk::Device nativeDevice, vk::Queue graphicsQueue, vk::CommandPool commandPool);
		Device(const Device& device);
		~Device();
		vk::Device getNativeDevice();
		vk::PhysicalDevice getPhysicalDevice();
		vk::Queue getGraphicsQueue();
		vk::CommandPool getCommandPool();
	private:
		vk::PhysicalDevice m_physicalDevice;
		vk::Device m_nativeDevice;
		vk::Queue m_graphicsQueue;
		vk::CommandPool m_commandPool;
	};
}

#endif // !KGS_DEVICE_H
