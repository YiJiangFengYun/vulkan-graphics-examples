#ifndef FW_APP_BASE_H
#define FW_APP_BASE_H

#include <memory>

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif // !GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "context.hpp"

namespace fw
{
	class AppBase
	{
	public:
		AppBase(int width, int height, const char *title);
		~AppBase();
		void virtual init();
		void virtual run();
	protected:

	private:
		int m_width;
		int m_height;
		const char *m_title;
		GLFWwindow *m_pWindow;
		std::unique_ptr<Context> m_context;
		void _initVulkan();
		void _onWindowResized(int width, int height);

		friend static void onWindowResized(GLFWwindow *window, int width, int height);
	};

	static void onWindowResized(GLFWwindow* window, int width, int height)
	{
		AppBase* const instance = (AppBase*)glfwGetWindowUserPointer(window);
		instance->_onWindowResized(width, height);
	}
}

#endif // !FW_APP_BASE_H
