#include "window.hpp"

namespace app
{
	Window::Window(uint32_t width, uint32_t height, const char* title,
		vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device,
		vk::Queue graphicsQueue, vk::Queue presentQueue):
		gfw::Window::Window(width, height, title, instance, physicalDevice, device, graphicsQueue, presentQueue)
	{

	}
}