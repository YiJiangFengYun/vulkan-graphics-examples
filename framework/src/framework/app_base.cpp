#include "app_base.hpp"

#include <plog/Log.h>
#include <plog/Appenders/DebugOutputAppender.h>

fw::AppBase::AppBase(int width, int height, const char *title)
	:m_width(width), m_height(height), m_title(title),
	m_pWindow(nullptr), m_context(nullptr)
{
}

fw::AppBase::~AppBase()
{
	if (m_pWindow != nullptr)
		glfwDestroyWindow(m_pWindow);

	glfwTerminate();
}

void fw::AppBase::init()
{
	LOG(plog::debug) << "Application initialization.";
	//init default log to write to the windows debug output
	static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
	plog::init(plog::verbose, &debugOutputAppender);

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	m_pWindow = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);

	glfwSetWindowUserPointer(m_pWindow, this);
	glfwSetWindowSizeCallback(m_pWindow, fw::onWindowResized);


	_initVulkan();

	LOG(plog::debug) << "Application initialization complete.";
}

void fw::AppBase::run()
{
	while (!glfwWindowShouldClose(m_pWindow))
	{
		glfwPollEvents();
	}
	//vkDeviceWaitIdle(device);
}

void fw::AppBase::_initVulkan()
{
	m_context = std::unique_ptr<Context>(new Context(m_title, VK_MAKE_VERSION(1, 0, 0), "No Engine", VK_MAKE_VERSION(1, 0, 0)));
	m_context->init();
}

void fw::AppBase::_onWindowResized(int width, int height)
{
	m_width = width;
	m_height = height;
}

void fw::onWindowResized(GLFWwindow* window, int width, int height)
{
	fw::AppBase* const instance = (fw::AppBase*)glfwGetWindowUserPointer(window);
	instance->_onWindowResized(width, height);
}