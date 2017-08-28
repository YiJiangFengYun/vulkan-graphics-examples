#ifndef KGS_DEVICE_H
#define KGS_DEVICE_H

#include <vulkan/vulkan.hpp>

namespace kgs
{
	class Device
	{
	public:
		Device(vk::Device nativeDevice);
		Device(const Device& device);
		~Device();
		vk::Device getNativeDevice();
	private:
		vk::Device m_nativeDevice;
	};
}

#endif // !KGS_DEVICE_H
