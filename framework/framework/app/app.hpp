#ifndef GFW_APP_BASE_H
#define GFW_APP_BASE_H

#include <memory>
#include <mutex>
#include <string>
#include "framework/global.hpp"
#include <foundation/foundation.hpp>
#include "framework/app/window.hpp"
#include "framework/util/thread_master.hpp"

#define DEFAULT_GRAPHICS_QUEUE_COUNT 10
#define DEFAULT_PRESENT_QUEUE_COUNT 5

namespace gfw
{

	class App
	{
	public:
		App();
		~App();
		void virtual run();
		template<typename MainWindow_T>
		void init(uint32_t width, uint32_t height, const char *title);
		template<typename MainWindow_T>
		void init(uint32_t width, uint32_t height, const char *title, Window::RenderType renderType);

		template<typename Window_T>
		void createSubWindow(uint32_t width, uint32_t height, const char *title);
		template<typename Window_T>
		void createSubWindow(uint32_t width, uint32_t height, const char *title, Window::RenderType renderType);

	protected:
		App(const App&) = delete;

		uint32_t m_width;
		uint32_t m_height;
		const char *m_title;
		uint32_t m_graphicsQueueCount;
		uint32_t m_presentQueuecount;
		std::shared_ptr<gfw::Window> m_pWindow;
		std::vector<std::shared_ptr<gfw::Window>> m_pSubWindows;

		//threads
		//ThreadMaster m_threadMaster; 
		//std::mutex m_subWindowsMutex;

		void _initEnv(uint32_t width
			, uint32_t height
			, const char *title
			, std::shared_ptr<GLFWwindow> &pResultGLFWWindow
			, std::shared_ptr<vk::SurfaceKHR> &pResultSurface);

		template <typename MainWindow_T>
		void _createWindow(std::shared_ptr<GLFWwindow> pWindow, std::shared_ptr<vk::SurfaceKHR> pSurface);
		template <typename MainWindow_T>
		void _createWindow(std::shared_ptr<GLFWwindow> pWindow, std::shared_ptr<vk::SurfaceKHR> pSurface, Window::RenderType renderType);

		//tool methods.
		std::shared_ptr<GLFWwindow> _createGLFWWindow(uint32_t width, uint32_t height, const char* title);
		std::shared_ptr<vk::SurfaceKHR> _createVKSurface(std::shared_ptr<GLFWwindow> pWindow);
	};
}

#include "framework/app/app.inl"

#endif // !GFW_APP_BASE_H
