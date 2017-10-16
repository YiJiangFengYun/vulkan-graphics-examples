namespace gfw
{
	template<typename MainWindow_T>
	void AppBase::init(uint32_t width, uint32_t height, const char *title)
	{
		LOG(plog::debug) << "Application initialization.";
		std::shared_ptr<GLFWwindow> pResultGLFWWindow;
		std::shared_ptr<vk::SurfaceKHR> pResultSurface;
		_initEnv(width, height, title, pResultGLFWWindow, pResultSurface);
		_createWindow<MainWindow_T>(pResultGLFWWindow, pResultSurface);
		LOG(plog::debug) << "Application initialization complete.";
	}

	template<typename Window_T>
	void AppBase::createSubWindow(uint32_t width, uint32_t height, const char *title)
	{
		std::shared_ptr<Window_T> window(new Window_T(width, height, title, m_pInstance, m_pPhysicalDevice,
			m_pDevice, m_graphicsQueue, m_presentQueue));
		/*{
		auto& subWindowsMutex = m_subWindowsMutex;
		std::lock_guard <std::mutex> lg(subWindowsMutex);
		std::vector<std::shared_ptr<Window>>& pSubWindows = m_pSubWindows;*/
		m_pSubWindows.push_back(window);

		//	//create new thread to run window.
		//	m_threadMaster.appendThread([&](std::shared_ptr<Window_T> window) {
		//		window->run();
		//		{
		//			std::lock_guard <std::mutex> lg(subWindowsMutex);
		//			pSubWindows.erase(window);
		//		}
		//	}, window);
		//}
	}

	template <typename MainWindow_T>
	void AppBase::_createWindow(std::shared_ptr<GLFWwindow> pWindow, std::shared_ptr<vk::SurfaceKHR> pSurface)
	{
		m_pWindow.reset(new MainWindow_T(pWindow, pSurface, m_pInstance, m_pPhysicalDevice, m_pDevice,
			m_graphicsQueue, m_presentQueue));
	}

} //namespace gfw
