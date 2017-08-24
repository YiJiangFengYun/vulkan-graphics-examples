#include "app/queue_family.hpp"

gfw::UsedQueueFamily gfw::UsedQueueFamily::findQueueFamilies(const vk::PhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
{
	UsedQueueFamily data;

	auto queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

	int i = 0;
	for (const auto& queueFamilyProperty : queueFamilyProperties)
	{
		if (queueFamilyProperty.queueCount > 0 && queueFamilyProperty.queueFlags & vk::QueueFlagBits::eGraphics)
		{
			data.graphicsFamily = i;
		}

		vk::Bool32 presentSupport = VK_FALSE;
		physicalDevice.getSurfaceSupportKHR(i, surface, &presentSupport);
		if (queueFamilyProperty.queueCount > 0 && presentSupport)
		{
			data.presentFamily = i;
		}

		if (data.isComplete()) {
			break;
		}

		++i;
	}

	return data;
}