#ifndef FW_APP_BASE_H
#define FW_APP_BASE_H

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif // !GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace fw
{
	class AppBase
	{
	public:
		void virtual init(int width, int height, const char *title);
		void virtual run();
	protected:

	private:
		int m_width;
		int m_height;
		const char *m_title;
		GLFWwindow *m_pWindow;

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
