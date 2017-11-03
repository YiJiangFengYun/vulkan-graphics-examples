#include "framework/app/app.hpp"

#include <plog/Log.h>
#include <plog/Appenders/DebugOutputAppender.h>
#include <set>
#include <thread>

namespace gfw {

	void runWindow(std::shared_ptr<Window> pSubWindow)
	{
		pSubWindow->run();
	}

	App::App()
		: m_width(0u)
		, m_height(0u)
		, m_title("")
		, m_graphicsQueueCount(DEFAULT_GRAPHICS_QUEUE_COUNT)
		, m_presentQueuecount(DEFAULT_PRESENT_QUEUE_COUNT)
	{

	}

	App::~App()
	{
		m_pWindow = nullptr;
		m_pSubWindows.resize(0);

		kgs::moduleDestory();

		glfwTerminate();
	}

	void App::run()
	{
		//std::thread thread_test;

		while (m_pWindow->windowShouldClose() == GFW_FALSE)
		{
			//remove closed child(sub) windows.
			m_pSubWindows.erase(std::remove_if(m_pSubWindows.begin(), m_pSubWindows.end(), [](const std::shared_ptr<Window>& item) {
				return item->windowShouldClose();
			}), m_pSubWindows.end());

			////multiply threads.
			//ThreadMaster threadMaster;
			//threadMaster.appendThread(std::shared_ptr<std::thread>(new std::thread(runWindow, m_pWindow)));
			//for (const auto& pSubWindow : m_pSubWindows)
			//{
			//	threadMaster.appendThread(std::shared_ptr<std::thread>(new std::thread(runWindow, pSubWindow)));
			//};
			//threadMaster.join();

			//single thread.
			m_pWindow->run();
			for (const auto& pSubWindow : m_pSubWindows)
			{
				pSubWindow->run();
			};
			kgs::pApp->getDevice()->waitIdle();
			glfwPollEvents();
		}
	}

	/*void AppBase::createSubWindow(uint32_t width, uint32_t height, const char *title)
	{
		_createSubWindow(width, height, title);
	}*/

	void App::_initEnv(uint32_t width
		, uint32_t height
		, const char *title
		, std::shared_ptr<GLFWwindow> &pResultGLFWWindow
	    , std::shared_ptr<vk::SurfaceKHR> &pResultSurface)
	{
		m_width = width;
		m_height = height;
		m_title = title;

		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		kgs::moduleCreateVkinstance("vulkan graphics", VK_MAKE_VERSION(1, 0, 0));

		pResultGLFWWindow = _createGLFWWindow(m_width, m_height, m_title);
		pResultSurface = _createVKSurface(pResultGLFWWindow);

		kgs::moduleCreateOther(pResultSurface, m_graphicsQueueCount, m_presentQueuecount);
	}

	std::shared_ptr<GLFWwindow> App::_createGLFWWindow(uint32_t width, uint32_t height, const char* title)
	{
		return fd::createGLFWWindow(width, height, title);
	}

	std::shared_ptr<vk::SurfaceKHR> App::_createVKSurface(std::shared_ptr<GLFWwindow> pWindow)
	{
		auto pInstance = kgs::pApp->getVKInstance();
		return fd::createSurface(pInstance, pWindow);
	}
}
