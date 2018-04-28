#include "graphics/util/queue_family.hpp"

namespace vg
{
	UsedQueueFamily::UsedQueueFamily()
		: graphicsFamily(-1)
	    , presentFamily(-1)
		, graphicsMaxQueueCount(0)
		, presentMaxQueueCount(0)
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
			if (queueFamilyProperty.queueCount > data.graphicsMaxQueueCount
				&& queueFamilyProperty.queueCount > 0 
				&& queueFamilyProperty.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				data.graphicsFamily = i;
				data.graphicsMaxQueueCount = queueFamilyProperty.queueCount;
			}

			vk::Bool32 presentSupport = VK_FALSE;
			physicalDevice.getSurfaceSupportKHR(i, surface, &presentSupport);
			if (queueFamilyProperty.queueCount > data.presentMaxQueueCount 
				&& queueFamilyProperty.queueCount > 0 
				&& presentSupport)
			{
				data.presentFamily = i;
				data.presentMaxQueueCount = queueFamilyProperty.queueCount;
			}

			++i;
		}

		return data;
	}
} //namespace kgs
