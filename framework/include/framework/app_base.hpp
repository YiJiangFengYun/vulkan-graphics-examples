#ifndef FW_APP_BASE_H
#define FW_APP_BASE_H

#include <memory>
#include "macro.hpp"
#include "context.hpp"
#include <GLFW/glfw3.h>

namespace fw
{
	class AppBase
	{
	public:
		AppBase(int32_t width, int32_t height, const char *title);
		~AppBase();
		void virtual init();
		void virtual run();
	protected:

	private:
		int32_t m_width;
		int32_t m_height;
		const char *m_title;
		GLFWwindow *m_pWindow;
		std::unique_ptr<Context> m_context;
		void _initVulkan();
		void _onWindowResized(int32_t width, int32_t height);

		friend void onWindowResized(GLFWwindow *window, int32_t width, int32_t height);
	};
}

#endif // !FW_APP_BASE_H
