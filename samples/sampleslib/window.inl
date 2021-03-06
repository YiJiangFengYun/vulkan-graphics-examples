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
        , m_rotationSpeed(0.0)
        , m_cameraZoom(0.0f)
        , m_cameraZoomSpeed(0.0)
        , m_cameraAspect(1.0f)
        , m_cameraPosition()
        , m_cameraRotation()
        , m_worldRotation()
        , m_mouseButtons()
        , m_mousePos()
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
        , m_sceneCount(1u)
        , m_preDepthScene(VG_FALSE)
    {
        
    }

    template <vg::SpaceType SPACE_TYPE>
    Window<SPACE_TYPE>::Window(std::shared_ptr<GLFWwindow> pWindow
        , std::shared_ptr<vk::SurfaceKHR> pSurface
    )
        : vgf::Window(pWindow
            , pSurface
        )
        , m_rotationSpeed(0.0)
        , m_cameraZoom(0.0f)
        , m_cameraZoomSpeed(0.0)
        , m_cameraAspect(1.0f)
        , m_cameraPosition()
        , m_cameraRotation()
        , m_worldRotation()
        , m_mouseButtons()
        , m_mousePos()
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
        , m_sceneCount(1u)    
    {
        
    }

    template <vg::SpaceType SPACE_TYPE>
    void Window<SPACE_TYPE>::_init()
    {
        _initState();
        _createCamera();
        _createScene();
        _initUI();
        _initInputHanders();
    }

    template <vg::SpaceType SPACE_TYPE>
    void Window<SPACE_TYPE>::_initState()
    {
        m_cameraZoom = 0.0f;
        m_cameraZoomSpeed = 0.1f;
        m_rotationSpeed = 0.01f;
        m_cameraAspect = (float)m_width / (float)m_height;
        m_sceneCount = 1u;
        m_cameraPosition = PointType(0.0f);
        m_cameraRotation = RotationDimType(0.0f);
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
        uint32_t sceneCount = m_sceneCount > 1u ? m_sceneCount : 1u;
        m_pScenes.resize(sceneCount);
        for (uint32_t i = 0; i < sceneCount; ++i)
        {
            m_pScenes[i] = std::shared_ptr<SceneType>(new SceneType());
            m_pScenes[i]->addCamera(m_pCamera.get());
        }
        m_pScene = m_pScenes[0];
    }

    template <vg::SpaceType SPACE_TYPE>
    void Window<SPACE_TYPE>::_initUI()
    {
#ifdef USE_IMGUI_BIND
        vgim::setShaderPath("shaders/ui.vert.spv", "shaders/ui.frag.spv");
#endif //USE_IMGUI_BIND
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

        for (uint32_t i = 0u; i < 3u; ++i) {
            vgf::Bool32 value = glfwGetMouseButton(m_pWindow.get(), i) != 0;
            memcpy(reinterpret_cast<vgf::Bool32 *>(&m_mouseButtons) + i,
                &value, sizeof(vgf::Bool32));
        }

#ifdef USE_IMGUI_BIND
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
        std::string vesion = ENGINE_VERSION;
        std::string vesionMajor = ENGINE_VERSION_MAJOR;
        std::string vesionMinor = ENGINE_VERSION_MINOR;
        std::string vesionPatch = ENGINE_VERSION_PATCH;
        std::string vesionExtra = ENGINE_VERSION_EXTRA;
        std::string vesionFull = ENGINE_VERSION_FULL;
        ImGui::Text("Framework Version: %s", ENGINE_VERSION_FULL);
        const auto &engineName = vg::pApp->getEngineName();
        ImGui::Text("Engine Name: %s", engineName.c_str());
        ImGui::Text("%.2f ms/frame (%.1d fps)", m_lastFPS == 0u ? 0.0f : (1000.0f / static_cast<float>(m_lastFPS)), m_lastFPS);
        ImGui::Text("Draw Count: %d", m_lastDrawCount);
        pos = ImGui::GetWindowPos();
        size = ImGui::GetWindowSize();
        ImGui::End();
        m_lastWinPos = vg::Vector2(pos.x, pos.y);
        m_lastWinSize = vg::Vector2(size.x, size.y);
#endif //USE_IMGUI_BIND
    }

    template <vg::SpaceType SPACE_TYPE>
    void Window<SPACE_TYPE>::_onPostUpdate()
    {

    }

    template <vg::SpaceType SPACE_TYPE>
    void Window<SPACE_TYPE>::_onPreDraw()
    {
        m_lastDrawCount = 0u;        
    }

    template <vg::SpaceType SPACE_TYPE>
    void Window<SPACE_TYPE>::_onDraw()
    {

    }

    template <vg::SpaceType SPACE_TYPE>
    void Window<SPACE_TYPE>::_onPostDraw()
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
    std::vector<vg::Renderer::SceneInfo> Window<SPACE_TYPE>::mySceneInfos = {};

    template <vg::SpaceType SPACE_TYPE>
    void Window<SPACE_TYPE>::_onPreRender(vg::Renderer::RenderInfo &info
        , vg::Renderer::RenderResultInfo &resultInfo)
    {
        uint32_t sceneCount = m_sceneCount;
        std::vector<vg::Renderer::SceneInfo> sceneInfos(sceneCount);
        for (uint32_t i = 0; i < sceneCount; ++i)
        {
            sceneInfos[i].pScene = m_pScenes[i].get();
            sceneInfos[i].pCamera = m_pCamera.get();
            sceneInfos[i].preDepth = m_preDepthScene;
        }
        uint32_t oldSceneCount = info.sceneInfoCount;
        info.sceneInfoCount = info.sceneInfoCount + sceneCount;
        Window<SPACE_TYPE>::mySceneInfos.resize(info.sceneInfoCount);
        uint32_t index = 0u;
        for (uint32_t i = 0; i < oldSceneCount; ++i)
        {
            mySceneInfos[index] = *(info.pSceneInfos + i);
            ++index;
        }

        for (uint32_t i = 0; i < sceneCount; ++i)
        {
            mySceneInfos[index] = sceneInfos[i];
            ++index;
        }
        
        info.pSceneInfos = mySceneInfos.data();
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
        m_lastDrawCount = resultInfo.drawCount;
    }
} //sampleslib