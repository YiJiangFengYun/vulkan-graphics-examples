#ifndef WINDOW_H
#define WINDOW_H

#include "framework/app/window.hpp"

namespace app
{
	class Window : public gfw::Window
	{
	public:
		Window(uint32_t width, uint32_t height, const char* title,
			std::shared_ptr<vk::Instance> pInstance, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
			std::shared_ptr<vk::Device> device, vk::Queue graphicsQueue, vk::Queue presentQueue);

	private:

	};

}

#endif // !WINDOW_H
