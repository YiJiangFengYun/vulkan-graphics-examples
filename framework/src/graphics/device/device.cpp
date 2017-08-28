#include "graphics/device/device.hpp"

namespace kgs
{
	Device::Device(vk::Device nativeDevice)
		:m_nativeDevice(nativeDevice)
	{
	}

	Device::Device(const Device& device)
		:m_nativeDevice(device.m_nativeDevice)
	{

	}

	Device::~Device()
	{
	}

	vk::Device Device::getNativeDevice()
	{
		return m_nativeDevice;
	}
}