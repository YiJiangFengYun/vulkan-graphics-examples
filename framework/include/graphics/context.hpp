#ifndef KGS_DEVICE_H
#define KGS_DEVICE_H

#include <vulkan/vulkan.hpp>
#include <foundation/foundation.hpp>

namespace kgs
{
	class Context
	{
	public:
		Context(std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
			,std::shared_ptr<vk::Device> pNativeDevice
			, vk::Queue graphicsQueue
			, uint32_t graphicsFamily
		);
		Context(const Context& device);
		~Context();
		std::shared_ptr<vk::PhysicalDevice> getPhysicalDevice();
		std::shared_ptr<vk::Device> getNativeDevice();
		vk::Queue getGraphicsQueue();
		std::shared_ptr<vk::CommandPool> getCommandPoolForTransientBuffer();
		std::shared_ptr<vk::CommandPool> getCommandPoolForResetBuffer();
	private:
		std::shared_ptr<vk::PhysicalDevice> m_pPhysicalDevice;
		std::shared_ptr<vk::Device> m_pNativeDevice;
		vk::Queue m_graphicsQueue;
		uint32_t m_graphicsFamily;

		std::shared_ptr<vk::CommandPool> m_pCommandPoolForTransientBuffer;
		std::shared_ptr<vk::CommandPool> m_pCommandPoolForResetBuffer;

		void _createCommandPool();
	};
}

#endif // !KGS_DEVICE_H
