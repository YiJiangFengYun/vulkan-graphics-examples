#include "graphics/device/device.hpp"

namespace kgs
{
	Device::Device(std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
		std::shared_ptr<vk::Device> pNativeDevice, vk::Queue graphicsQueue,
		std::shared_ptr<vk::CommandPool> pCommandPool)
		:m_pPhysicalDevice(pPhysicalDevice),
		m_pNativeDevice(pNativeDevice),
		m_graphicsQueue(graphicsQueue),
		m_pCommandPool(pCommandPool)
	{
	}

	Device::Device(const Device& device)
		:m_pPhysicalDevice(device.m_pPhysicalDevice),
		m_pNativeDevice(device.m_pNativeDevice),
		m_graphicsQueue(device.m_graphicsQueue),
		m_pCommandPool(device.m_pCommandPool)
	{

	}

	Device::~Device()
	{
	}

	std::shared_ptr<vk::PhysicalDevice> Device::getPPhysicalDevice()
	{
		return m_pPhysicalDevice;
	}

	std::shared_ptr<vk::Device> Device::getPNativeDevice()
	{
		return m_pNativeDevice;
	}

	vk::Queue Device::getGraphicsQueue()
	{
		return m_graphicsQueue;
	}

	std::shared_ptr<vk::CommandPool> Device::getPCommandPool()
	{
		return m_pCommandPool;
	}
}