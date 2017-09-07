#include "graphics/context.hpp"

namespace kgs
{
	Context::Context(std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
		std::shared_ptr<vk::Device> pNativeDevice, vk::Queue graphicsQueue,
		std::shared_ptr<vk::CommandPool> pCommandPool)
		:m_pPhysicalDevice(pPhysicalDevice),
		m_pNativeDevice(pNativeDevice),
		m_graphicsQueue(graphicsQueue),
		m_pCommandPool(pCommandPool)
	{
	}

	Context::Context(const Context& device)
		:m_pPhysicalDevice(device.m_pPhysicalDevice),
		m_pNativeDevice(device.m_pNativeDevice),
		m_graphicsQueue(device.m_graphicsQueue),
		m_pCommandPool(device.m_pCommandPool)
	{

	}

	Context::~Context()
	{
	}

	std::shared_ptr<vk::PhysicalDevice> Context::getPPhysicalDevice()
	{
		return m_pPhysicalDevice;
	}

	std::shared_ptr<vk::Device> Context::getPNativeDevice()
	{
		return m_pNativeDevice;
	}

	vk::Queue Context::getGraphicsQueue()
	{
		return m_graphicsQueue;
	}

	std::shared_ptr<vk::CommandPool> Context::getPCommandPool()
	{
		return m_pCommandPool;
	}
}