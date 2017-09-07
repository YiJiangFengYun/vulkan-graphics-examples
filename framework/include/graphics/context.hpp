#ifndef KGS_DEVICE_H
#define KGS_DEVICE_H

#include <vulkan/vulkan.hpp>

namespace kgs
{
	class Context
	{
	public:
		Context(std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
			std::shared_ptr<vk::Device> pNativeDevice, vk::Queue graphicsQueue, 
			std::shared_ptr<vk::CommandPool> pCommandPool);
		Context(const Context& device);
		~Context();
		std::shared_ptr<vk::PhysicalDevice> getPPhysicalDevice();
		std::shared_ptr<vk::Device> getPNativeDevice();
		vk::Queue getGraphicsQueue();
		std::shared_ptr<vk::CommandPool> getPCommandPool();
	private:
		std::shared_ptr<vk::PhysicalDevice> m_pPhysicalDevice;
		std::shared_ptr<vk::Device> m_pNativeDevice;
		vk::Queue m_graphicsQueue;
		std::shared_ptr<vk::CommandPool> m_pCommandPool;
	};
}

#endif // !KGS_DEVICE_H
