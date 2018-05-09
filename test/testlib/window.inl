#include <iostream>

namespace testlib
{
	template <vg::SpaceType SPACE_TYPE>
	Window<SPACE_TYPE>::Window(uint32_t width
		, uint32_t height
		, const char* title
	)
		: vgf::Window(width
			, height
			, title
		    )
		, m_zoom(0.0f)
		, m_rotation()
	{
		_initZoom();
		_createCamera();
		_createScene();
		_initInputHanders();
	}

	template <vg::SpaceType SPACE_TYPE>
	Window<SPACE_TYPE>::Window(std::shared_ptr<GLFWwindow> pWindow
		, std::shared_ptr<vk::SurfaceKHR> pSurface
	)
		: vgf::Window(pWindow
			, pSurface
		    )
		, m_zoom(0.0f)
		, m_rotation()
	{
		_initZoom();
		_createCamera();
		_createScene();
		_initInputHanders();
	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_initZoom()
	{
		m_zoom = -2.5f;
		m_zoomSpeed = 0.5f;
	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_createCamera()
	{
		m_pCamera = std::shared_ptr<CameraType>(new CameraType());
		_updateCamera();
	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_createScene()
	{
		m_pScene = std::shared_ptr<SceneType>(new SceneType());
		m_pScene->addCamera(m_pCamera.get());
	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_initInputHanders()
	{
		glfwSetScrollCallback(m_pWindow.get(), [](GLFWwindow *window, double xOffset, double yOffset)
		{
			Window* const instance = (Window*)glfwGetWindowUserPointer(window);
			instance->m_zoom += static_cast<float>(yOffset) * instance->m_zoomSpeed;
			instance->m_zoom = std::min(-0.15f, std::max(-5.0f, instance->m_zoom));
			std::cout << "Current zoom: " << instance->m_zoom << std::endl;
		});
	}	

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onResize()
	{
		_updateCamera();
	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onPreReCreateSwapchain()
	{

	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onPostReCreateSwapchain()
	{
	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onPreUpdate()
	{

	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onUpdate()
	{
		_updateCamera();
	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onPostUpdate()
	{

	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onPreDraw()
	{

	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onDraw()
	{

	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onPostDraw()
	{

	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onPreRender(vg::Renderer::RenderInfo &info
		, vg::Renderer::RenderResultInfo &resultInfo)
	{
		vg::Renderer::SceneInfo sceneInfo;
		sceneInfo.pScene = m_pScene.get();
		sceneInfo.pCamera = m_pCamera.get();
		uint32_t oldSceneCount = info.sceneInfoCount;
		info.sceneInfoCount = info.sceneInfoCount + 1u;
		std::vector<vg::Renderer::SceneInfo> sceneInfos(info.sceneInfoCount);
		for (uint32_t i = 0; i < oldSceneCount; ++i)
		{
			sceneInfos[i] = *(info.pSceneInfos + i);
		}
		sceneInfos[info.sceneInfoCount] = sceneInfo;
		info.pSceneInfos = sceneInfos.data();
	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onRender(vg::Renderer::RenderInfo &info
		, vg::Renderer::RenderResultInfo &resultInfo)
	{

	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onPostRender(vg::Renderer::RenderInfo &info
		, vg::Renderer::RenderResultInfo &resultInfo)
	{

	}
}