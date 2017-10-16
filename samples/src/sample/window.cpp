#include "sample/window.hpp"

namespace app
{
	Window::Window(uint32_t width, uint32_t height, const char* title,
		std::shared_ptr<vk::Instance> pInstance, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
		std::shared_ptr<vk::Device> device, vk::Queue graphicsQueue, vk::Queue presentQueue)
		: gfw::Window(width, height, title, pInstance, pPhysicalDevice, device, graphicsQueue, presentQueue)
	{

	}

	Window::Window(std::shared_ptr<GLFWwindow> pWindow, std::shared_ptr<vk::SurfaceKHR> pSurface,
		std::shared_ptr<vk::Instance> pInstance, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
		std::shared_ptr<vk::Device> pDevice, vk::Queue graphicsQueue, vk::Queue presentQueue)
		: gfw::Window(pWindow, pSurface, pInstance, pPhysicalDevice, pDevice, graphicsQueue, presentQueue)
	{

	}

	void Window::_onPreUpdate()
	{

	}

	void Window::_onPostUpdate()
	{

	}

	void Window::_onPreRender()
	{

	}

	void Window::_onPostRender()
	{

	}
}