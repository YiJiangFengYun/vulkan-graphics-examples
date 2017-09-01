#include "graphics/device/device.hpp"

namespace kgs
{
	Device::Device(vk::PhysicalDevice physicalDevice, vk::Device nativeDevice, vk::Queue graphicsQueue, vk::CommandPool commandPool)
		:m_physicalDevice(physicalDevice),
		m_nativeDevice(nativeDevice),
		m_graphicsQueue(graphicsQueue),
		m_commandPool(commandPool)
	{
	}

	Device::Device(const Device& device)
		:m_nativeDevice(device.m_nativeDevice)
	{

	}

	Device::~Device()
	{
	}

	vk::PhysicalDevice Device::getPhysicalDevice()
	{
		return m_physicalDevice;
	}

	vk::Device Device::getNativeDevice()
	{
		return m_nativeDevice;
	}

	vk::Queue Device::getGraphicsQueue()
	{
		return m_graphicsQueue;
	}

	vk::CommandPool Device::getCommandPool()
	{
		return m_commandPool;
	}
}