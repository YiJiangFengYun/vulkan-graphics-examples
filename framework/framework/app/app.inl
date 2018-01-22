namespace vgf
{
	template<typename MainWindow_T>
	void App::init(uint32_t width, uint32_t height, const char *title)
	{
		LOG(plog::debug) << "Application initialization.";
		std::shared_ptr<GLFWwindow> pResultGLFWWindow;
		std::shared_ptr<vk::SurfaceKHR> pResultSurface;
		_initEnv(width, height, title, pResultGLFWWindow, pResultSurface);
		_createWindow<MainWindow_T>(pResultGLFWWindow, pResultSurface);
		LOG(plog::debug) << "Application initialization complete.";
	}

	template<typename MainWindow_T>
	void App::init(uint32_t width, uint32_t height, const char *title, Window::RenderType renderType)
	{
		LOG(plog::debug) << "Application initialization.";
		std::shared_ptr<GLFWwindow> pResultGLFWWindow;
		std::shared_ptr<vk::SurfaceKHR> pResultSurface;
		_initEnv(width, height, title, pResultGLFWWindow, pResultSurface);
		_createWindow<MainWindow_T>(pResultGLFWWindow, pResultSurface, renderType);
		LOG(plog::debug) << "Application initialization complete.";
	}

	template<typename Window_T>
	void App::createSubWindow(uint32_t width, uint32_t height, const char *title)
	{
		std::shared_ptr<Window_T> window(new Window_T(width
			, height
			, title));
		m_pSubWindows.push_back(window);
	}

	template<typename Window_T>
	void App::createSubWindow(uint32_t width, uint32_t height, const char *title, Window::RenderType renderType)
	{
		std::shared_ptr<Window_T> window(new Window_T(width
			, height
			, title
			, renderType));
		m_pSubWindows.push_back(window);
	}

	template <typename MainWindow_T>
	void App::_createWindow(std::shared_ptr<GLFWwindow> pWindow, std::shared_ptr<vk::SurfaceKHR> pSurface)
	{
		m_pWindow.reset(new MainWindow_T(pWindow
			, pSurface));
	}

	template <typename MainWindow_T>
	void App::_createWindow(std::shared_ptr<GLFWwindow> pWindow, std::shared_ptr<vk::SurfaceKHR> pSurface, Window::RenderType renderType)
	{
		m_pWindow.reset(new MainWindow_T(renderType
			, pWindow
			, pSurface));
	}

} //namespace gfw
