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
		, m_startTimeFrame()
		, m_endTimeFrame()
		, m_isPause(false)
		, m_frameTimer(0.0f)
		, m_passedTime(0.0f)
		, m_timerSpeedFactor(1.0f)
		, m_fpsTimer(0u)		
		, m_frameCounter(0u)
		, m_lastFPS(0u)
		, m_lastDrawCount(0u)
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
		, m_startTimeFrame()
		, m_endTimeFrame()
		, m_isPause(false)
		, m_frameTimer(0.0f)
		, m_passedTime(0.0f)
		, m_timerSpeedFactor(1.0f)
		, m_fpsTimer(0u)
		, m_frameCounter(0u)
		, m_lastFPS(0u)
		, m_lastDrawCount(0u)		
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
		m_startTimeFrame = std::chrono::high_resolution_clock::now();
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
		ImGui::Text("App Name: %s", appName.c_str());
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
		ImGui::Text("Engine Name: %s", engineName.c_str());
		const auto &engineVersion = vg::pApp->getEngineVersion();
		std::string vgVesion = VG_VERSION;
		std::string vgVesionMajor = VG_VERSION_MAJOR;
		std::string vgVesionMinor = VG_VERSION_MINOR;
		std::string vgVesionPatch = VG_VERSION_PATCH;
		std::string vgVesionExtra = VG_VERSION_EXTRA;
		std::string vgVesionFull = VG_VERSION_FULL;
		ImGui::Text("Engine Version: %s", VG_VERSION_FULL);
		ImGui::Text("%.2f ms/frame (%.1d fps)", m_lastFPS == 0u ? 0.0f : (1000.0f / static_cast<float>(m_lastFPS)), m_lastFPS);
		ImGui::Text("Draw Count: %d", m_lastDrawCount);
		ImGui::End();
	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onPostUpdate()
	{

	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onPreRender()
	{
		m_lastDrawCount = 0u;		
	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onRender()
	{

	}

	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_onPostRender()
	{
		auto now = std::chrono::high_resolution_clock::now();
		m_endTimeFrame = now;
		auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_startTimeFrame).count();
		m_frameTimer = diff  / 1000.0f;
		if (m_isPause == false) {
		    m_passedTime += m_frameTimer * m_timerSpeedFactor;
		}
		m_fpsTimer += static_cast<uint32_t>(diff);
		++m_frameCounter;
		if (m_fpsTimer > 1000u)
		{
			m_lastFPS = static_cast<uint32_t>(static_cast<float>(m_frameCounter) * (1000.0f / m_fpsTimer));
			m_fpsTimer = 0u;
			m_frameCounter = 0u;
		}
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
		m_lastDrawCount += resultInfo.drawCount;
	}
} //sampleslib