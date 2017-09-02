#include "sample/window.hpp"

namespace app
{
	Window::Window(uint32_t width, uint32_t height, const char* title,
		std::shared_ptr<vk::Instance> pInstance, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
		std::shared_ptr<vk::Device> device, vk::Queue graphicsQueue, vk::Queue presentQueue):
		gfw::Window::Window(width, height, title, pInstance, pPhysicalDevice, device, graphicsQueue, presentQueue)
	{

	}
}