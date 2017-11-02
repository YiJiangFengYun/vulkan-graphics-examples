#include "graphics/util/queue_family.hpp"

namespace kgs
{
	UsedQueueFamily::UsedQueueFamily()
		: graphicsFamily(-1)
	    , presentFamily(-1)
	{

	}

	Bool32 UsedQueueFamily::isComplete() const
	{
		return graphicsFamily >= 0 && presentFamily >= 0;
	}

	UsedQueueFamily UsedQueueFamily::findQueueFamilies(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface)
	{
		UsedQueueFamily data;

		auto queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

		int i = 0;
		for (const auto& queueFamilyProperty : queueFamilyProperties)
		{
			if (queueFamilyProperty.queueCount > 0 && queueFamilyProperty.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				data.graphicsFamily = i;
				data.graphicsMaxQueueCount = queueFamilyProperty.queueCount;
			}

			vk::Bool32 presentSupport = VK_FALSE;
			physicalDevice.getSurfaceSupportKHR(i, surface, &presentSupport);
			if (queueFamilyProperty.queueCount > 0 && presentSupport)
			{
				data.presentFamily = i;
				data.presentMaxQueueCount = queueFamilyProperty.queueCount;
			}

			if (data.isComplete()) {
				break;
			}

			++i;
		}

		return data;
	}
} //namespace kgs
