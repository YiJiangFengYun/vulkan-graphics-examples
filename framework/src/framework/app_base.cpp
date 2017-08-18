#include "app_base.hpp"

#include <plog/Log.h>
#include <plog/Appenders/DebugOutputAppender.h>

void fw::AppBase::init(int width, int height, const char *title)
{
	m_width = width;
	m_height = height;
	m_title = title;
	//init default log to write to the windows debug output
	static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
	plog::init(plog::verbose, &debugOutputAppender);

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	m_pWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

	glfwSetWindowUserPointer(m_pWindow, this);
	glfwSetWindowSizeCallback(m_pWindow, fw::onWindowResized);

	LOG(plog::debug) << "Application initialization complete.";
}

void fw::AppBase::run()
{
	while (true)
	{

	}
}

void fw::AppBase::_onWindowResized(int width, int height)
{
	m_width = width;
	m_height = height;
}