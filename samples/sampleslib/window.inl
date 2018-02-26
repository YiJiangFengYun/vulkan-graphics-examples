#include <iostream>

namespace sampleslib
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
		_initUI();
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
		_initUI();
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
		m_pScene->addCamera(m_pCamera);
	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_initUI()
	{
		vgim::setShaderPath("shaders/ui.vert.spv", "shaders/ui.frag.spv");
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
		ImGui::SetNextWindowPos(ImVec2(10, 10));
	    ImGui::SetNextWindowSize(ImVec2(0, 0));
	    ImGui::Begin("Device Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		const auto &physicalDevice = vg::pApp->getPhysicalDevice();
		const auto &deviceProperties = physicalDevice->getProperties();
	    ImGui::Text("Device Name: %s", deviceProperties.deviceName);

		auto pos = ImGui::GetWindowPos();
		auto size = ImGui::GetWindowSize();
		ImGui::End();
		ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y + size.y + 10));
	    ImGui::SetNextWindowSize(ImVec2(0, 0));
	    ImGui::Begin("App Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		const auto &appName = vg::pApp->getAppName();
		ImGui::Text("App Name: %s", appName);
		const auto &appVersion = vg::pApp->getAppVersion();
		uint32_t appVersionMajor = VG_GET_VERSION_MAJOR(appVersion);
		uint32_t appVersionMinor = VG_GET_VERSION_MINOR(appVersion);
		uint32_t appVersionPatch = VG_GET_VERSION_PATCH(appVersion);
		ImGui::Text("App Version: %d.%d.%d", appVersionMajor, appVersionMinor, appVersionPatch);
		std::string vgfVesion = VGF_VERSION;
		std::string vgfVesionMajor = VGF_VERSION_MAJOR;
		std::string vgfVesionMinor = VGF_VERSION_MINOR;
		std::string vgfVesionPatch = VGF_VERSION_PATCH;
		std::string vgfVesionExtra = VGF_VERSION_EXTRA;
		std::string vgfVesionFull = VGF_VERSION_FULL;
		ImGui::Text("Framework Version: %s", VGF_VERSION_FULL);
		const auto &engineName = vg::pApp->getEngineName();
		ImGui::Text("Engine Name: %s", engineName);
		const auto &engineVersion = vg::pApp->getEngineVersion();
		uint32_t engineVersionMajor = VG_GET_VERSION_MAJOR(engineVersion);
		uint32_t engineVersionMinor = VG_GET_VERSION_MINOR(engineVersion);
		uint32_t engineVersionPatch = VG_GET_VERSION_PATCH(engineVersion);
		ImGui::Text("Engine Version: %d.%d.%d", engineVersionMajor, engineVersionMinor, engineVersionPatch);
		ImGui::End();
	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onPostUpdate()
	{

	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onPreRender()
	{

	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onRender()
	{

	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onPostRender()
	{

	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_renderWithRenderer(const std::shared_ptr<vg::Renderer> &pRenderer
		    , const vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo)
	{
		vg::Renderer::SceneAndCamera sceneAndCamera;
		sceneAndCamera.pScene = m_pScene.get();
		sceneAndCamera.pCamera = m_pCamera.get();
		auto myInfo = info;
		myInfo.sceneAndCameraCount = myInfo.sceneAndCameraCount + 1u;
		std::vector<vg::Renderer::SceneAndCamera> sceneAndCameras(myInfo.sceneAndCameraCount);
		for (uint32_t i = 0; i < info.sceneAndCameraCount; ++i)
		{
			sceneAndCameras[i] = *(info.pSceneAndCamera + i);
		}
		sceneAndCameras[info.sceneAndCameraCount] = sceneAndCamera;
		myInfo.pSceneAndCamera = sceneAndCameras.data();
		vgf::Window::_renderWithRenderer(pRenderer, myInfo, resultInfo);

		// vgf::Window::_renderWithRenderer(pRenderer, info, resultInfo);		
	}
} //sampleslib