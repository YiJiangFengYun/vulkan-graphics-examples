namespace gfw
{
	inline void Window::_preUpdate()
	{
		_onPreUpdate();
	}

	inline void Window::_update()
	{
		_onUpdate();
	}
	inline void Window::_postUpdate()
	{
		_postUpdate();
	}

	void Window::_preRender()
	{
		_onPreRender();


		/*uint32_t imageIndex = m_pDevice->acquireNextImageKHR(*m_pSwapchain,
		std::numeric_limits<uint64_t>::max(),
		*m_pImageAvailableSemaphore, nullptr).value;*/
	}

	void Window::_render()
	{
		_onRender();
	}

	void Window::_postRender()
	{
		_onPostRender();
	}
} //namespace gfw