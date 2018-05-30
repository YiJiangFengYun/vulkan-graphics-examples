#include "pipelines/window.hpp"

#include <iostream>


Window::Window(uint32_t width
    , uint32_t height
    , const char* title
)
    : sampleslib::Window<vg::SpaceType::SPACE_3>(width
        , height
        , title
        )
    , m_assimpScene()
    , m_pMaterials()
{
    _init();
}

Window::Window(std::shared_ptr<GLFWwindow> pWindow
    , std::shared_ptr<vk::SurfaceKHR> pSurface
)
    : sampleslib::Window<vg::SpaceType::SPACE_3>(pWindow
        , pSurface
        )
    , m_assimpScene()
    , m_pMaterials()
{
    _init();
}

void Window::_init()
{
    ParentWindowType::_init();
    _loadAssimpScene();    
    _createMaterial();
    _createVisualObjects();
    _fillScene();
}

void Window::_initState()
{
    ParentWindowType::_initState();
    m_sceneCount = SCENE_COUNT;

    m_cameraAspect = (float)m_width / 3.0f / (float)m_height;
    m_cameraZoom = -10.5f;
    /// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
    m_cameraRotation = vg::Vector3(glm::radians(0.0f), glm::radians(15.0f), glm::radians(0.0f));

    m_lightInfo.lightPos = vg::Vector4(0.0f, 2.0f, -1.0f, 1.0f);
}

void Window::_loadAssimpScene()
{
    const uint32_t layoutCount = 4u;
    sampleslib::AssimpScene::VertexLayoutComponent layouts[layoutCount] = {
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_POSITION,
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_NORMAL,
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_UV,
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_COLOR                        
    };
    sampleslib::AssimpScene::CreateInfo createInfo;
    createInfo.fileName = "models/treasure_smooth.dae";
    createInfo.isCreateObject = VG_TRUE;
    createInfo.layoutComponentCount = layoutCount;
    createInfo.pLayoutComponent = layouts;
    createInfo.offset = vg::Vector3(3.0f, -1.2f, -1.0f);
    createInfo.isCreateObject = VG_FALSE;
    m_assimpScene.init(createInfo);
}

void Window::_createMaterial()
{
    std::string vertShaderPaths[SCENE_COUNT] = {
        "shaders/pipelines/phong.vert.spv",
        "shaders/pipelines/toon.vert.spv",
        "shaders/pipelines/wireframe.vert.spv"
    };

    std::string fragShaderPaths[SCENE_COUNT] = {
        "shaders/pipelines/phong.frag.spv",
        "shaders/pipelines/toon.frag.spv",
        "shaders/pipelines/wireframe.frag.spv"
    };

    auto& pMaterials = m_pMaterials;
    auto& pApp = vg::pApp;
    for (uint32_t i = 0; i < SCENE_COUNT; ++i)
    {
        //material
        pMaterials[i] = std::shared_ptr<vg::Material>(new vg::Material());
        pMaterials[i]->setRenderPriority(0u);
        pMaterials[i]->setRenderQueueType(vg::MaterialShowType::OPAQUE);

        auto pShader = pMaterials[i]->getMainShader();
        auto pPass = pMaterials[i]->getMainPass();
        //shader
        pShader->load(vertShaderPaths[i], fragShaderPaths[i]);
        //pass
        pPass->setCullMode(vk::CullModeFlagBits::eBack);
        pPass->setFrontFace(vk::FrontFace::eClockwise);
        pPass->setViewport(fd::Viewport(static_cast<float>(i) / static_cast<float>(SCENE_COUNT),
             0.0f,
             1.0f / static_cast<float>(SCENE_COUNT),
             1.0f));
        vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
        depthStencilState.depthTestEnable = VG_TRUE;
        depthStencilState.depthWriteEnable = VG_TRUE;
        depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
        pPass->setDepthStencilInfo(depthStencilState);
        if (i == 1u && pApp->getPhysicalDeviceFeatures().wideLines)
        {
            pPass->setLineWidth(2.0f);
        }

        if (i == 2u)
        {
            pPass->setPolygonMode(vk::PolygonMode::eLine);
        }

        vg::PassDataInfo lightDataInfo = {
            VG_PASS_OTHER_DATA_MIN_LAYOUT_PRIORITY,
            vk::ShaderStageFlagBits::eVertex,
        };

        pPass->addData("light_info", lightDataInfo, m_lightInfo);
        pPass->apply();

        
        pMaterials[i]->apply();
    }

}

void Window::_createVisualObjects()
{
    const auto &meshes = m_assimpScene.getMeshes();
    if (meshes.size() > 1) throw std::runtime_error("Mesh count more than 1 in assimp scene.");
    for (uint32_t i = 0; i < SCENE_COUNT; ++i)
    {
        m_pVisualObjects[i] = std::shared_ptr<vg::VisualObject3>{new vg::VisualObject3()};
        m_pVisualObjects[i]->setMesh(meshes[0].get());
        auto pMaterial = m_pMaterials[i].get();
        m_pVisualObjects[i]->setMaterialCount(1u);
        m_pVisualObjects[i]->setMaterial(pMaterial);
    }
}

void Window::_fillScene()
{
    for (uint32_t i = 0; i < SCENE_COUNT; ++i)
    {
        m_pScenes[i]->addVisualObject(m_pVisualObjects[i].get());
    }
}

void Window::_onUpdate()
{
    ParentWindowType::_onUpdate();
    const auto &pApp = vg::pApp;
    if (pApp->getPhysicalDeviceFeatures().fillModeNonSolid == VK_FALSE)
    {
#ifdef USE_IMGUI_BIND
        auto pos = ImGui::GetWindowPos();
        auto size = ImGui::GetWindowSize();
        ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y + size.y + 10));
        ImGui::SetNextWindowSize(ImVec2(0, 0));
        ImGui::Begin("Extra Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::TextUnformatted("Non solid fill modes not supported!");
        ImGui::End();
#endif //USE_IMGUI_BIND
    }
}