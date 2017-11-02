#include "graphics/context.hpp"

namespace kgs
{
	Context::Context(std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
		, std::shared_ptr<vk::Device> pNativeDevice, vk::Queue graphicsQueue
		, uint32_t graphicsFamily
	)
		: m_pPhysicalDevice(pPhysicalDevice)
		, m_pNativeDevice(pNativeDevice)
		, m_graphicsQueue(graphicsQueue)
		, m_graphicsFamily(graphicsFamily)
	{
		_init();
	}

	Context::Context(const Context& device)
		: m_pPhysicalDevice(device.m_pPhysicalDevice)
		, m_pNativeDevice(device.m_pNativeDevice)
		, m_graphicsQueue(device.m_graphicsQueue)
		, m_graphicsFamily(device.m_graphicsFamily)
	{
		_init();
	}

	Context::~Context()
	{
	}

	std::shared_ptr<vk::PhysicalDevice> Context::getPhysicalDevice()
	{
		return m_pPhysicalDevice;
	}

	std::shared_ptr<vk::Device> Context::getNativeDevice()
	{
		return m_pNativeDevice;
	}

	vk::Queue Context::getGraphicsQueue()
	{
		return m_graphicsQueue;
	}

	uint32_t Context::getGraphicsFamily()
	{
		return m_graphicsFamily;
	}

	std::shared_ptr<vk::CommandPool> Context::getCommandPoolForTransientBuffer()
	{
		return m_pCommandPoolForTransientBuffer;
	}

	std::shared_ptr<vk::CommandPool> Context::getCommandPoolForResetBuffer()
	{
		return m_pCommandPoolForResetBuffer;
	}

	void Context::_createCommandPool()
	{
		vk::CommandPoolCreateInfo createInfoForTransientBuffer = {
			vk::CommandPoolCreateFlagBits::eTransient,
			m_graphicsFamily
		};
		m_pCommandPoolForTransientBuffer = fd::createCommandPool(m_pNativeDevice, createInfoForTransientBuffer);

		vk::CommandPoolCreateInfo createInfoForResetBuffer = {
			vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
			m_graphicsFamily
		};
		m_pCommandPoolForResetBuffer = fd::createCommandPool(m_pNativeDevice, createInfoForResetBuffer);
	}
}