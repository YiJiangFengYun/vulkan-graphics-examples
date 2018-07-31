#include "spot_light/window.hpp"

#include <iostream>
#include <typeinfo>


Window::Window(uint32_t width
    , uint32_t height
    , const char* title
)
    : sampleslib::Window<vg::SpaceType::SPACE_3>(width
        , height
        , title
        )
    , m_assimpScene()
    , m_pMaterial()
    , m_pSpotLight()
    , m_lightRange(DEFAULT_LIGHT_RANGE)
    , m_lightStrength(1.0f)
    , m_pAmbientLight()
    , m_ambientLightStrength(0.1f)
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
    , m_pMaterial()
    , m_pSpotLight()
    , m_lightRange(DEFAULT_LIGHT_RANGE)
    , m_lightStrength(1.0f)
    , m_pAmbientLight()
    , m_ambientLightStrength(0.1f)
{
    _init();
}

void Window::_init()
{
    ParentWindowType::_init();
    _createModel();
    _createLights();
    _createMaterial();
    _initScene();
    _enableLighting();
    _enableShadow();
}

void Window::_initState()
{
    ParentWindowType::_initState();
    m_cameraZoom = -15.0f;
    /// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
    m_cameraRotation = vg::Vector3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
    m_timerSpeedFactor = 0.1f;
    m_lightRange = DEFAULT_LIGHT_RANGE;
}

void Window::_createModel()
{
    const uint32_t layoutCount = 4u;
    sampleslib::AssimpScene::VertexLayoutComponent layouts[layoutCount] = {
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_POSITION,
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_UV,
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_COLOR,
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_NORMAL,
    };
    sampleslib::AssimpScene::CreateInfo createInfo;
    createInfo.fileName = "models/vulkanscene_shadow.dae";
    createInfo.isCreateObject = VG_TRUE;
    createInfo.layoutComponentCount = layoutCount;
    createInfo.pLayoutComponent = layouts;
    createInfo.offset = vg::Vector3(0.0f, 0.0f, 0.0f);
    createInfo.scale = vg::Vector3(4.0f);
    m_assimpScene.init(createInfo);
}

void Window::_createLights()
{
    m_pSpotLight = std::shared_ptr<vg::LightSpot3>{ new vg::LightSpot3(glm::radians(45.0f), m_lightRange, 2048, 2048, vk::Format::eD16Unorm)};
    m_pSpotLight->setStrength(m_lightStrength);

    m_pAmbientLight = std::shared_ptr<vg::LightAmbient3>{ new vg::LightAmbient3()};
    m_pAmbientLight->setStrength(m_ambientLightStrength);
    _updateLights();
}

void Window::_createMaterial()
{
    auto & pApp = vg::pApp;
    {
        //material
        vg::Material::MaterialCreateInfo createInfo = {
            VG_FALSE,
        };

        auto & pMaterial = m_pMaterial;
        pMaterial = std::shared_ptr<vg::Material>(new vg::Material(createInfo));
        pMaterial->setRenderPriority(0u);
        pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);

        auto pShader = pMaterial->getMainShader();
        auto pPass = pMaterial->getMainPass();
        
        //shader
        pShader->load("shaders/spot_light/scene.vert.spv",
            "shaders/spot_light/scene.frag.spv");
        //pass
        vg::Pass::BuildInDataInfo::Component buildInDataCmps[3] = {
                {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
                {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD},
                {vg::Pass::BuildInDataType::POS_VIEWER},
            };
        vg::Pass::BuildInDataInfo buildInDataInfo;
        buildInDataInfo.componentCount = 3u;
        buildInDataInfo.pComponent = buildInDataCmps;
        pPass->setBuildInDataInfo(buildInDataInfo);
        pPass->setCullMode(vk::CullModeFlagBits::eBack);
        pPass->setFrontFace(vk::FrontFace::eClockwise);
        vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
        depthStencilState.depthTestEnable = VG_TRUE;
        depthStencilState.depthWriteEnable = VG_TRUE;
        depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
        pPass->setDepthStencilInfo(depthStencilState);
        pPass->apply();
        
        pMaterial->apply();
    }

}

void Window::_initScene()
{
    const auto &objects = m_assimpScene.getObjects();
    for (const auto &object : objects)
    {
        object->setMaterialCount(1u);
        object->setMaterial(m_pMaterial.get());
        object->setLightingMaterial(typeid(vg::LightSpot3), vg::pDefaultLightingDepthMaterial.get());
        m_pScene->addVisualObject(object.get());
    }

    {
        const auto &lightTypeInfo = typeid(vg::LightSpot3);
        vg::SceneLightRegisterInfo registerInfo = {
            0u,
            MAX_LIGHT_COUNT,
            vg::LightSpot3::registerInfo.dataSize,
            vg::LightSpot3::registerInfo.textureCount,
        };
        m_pScene->registerLight(lightTypeInfo, registerInfo);
        m_pScene->addLight(m_pSpotLight.get());
    }

    {
        const auto &lightTypeInfo = typeid(vg::LightAmbient3);
        vg::SceneLightRegisterInfo registerInfo = {
            1u,
            1u,
            vg::LightAmbient3::registerInfo.dataSize,
            vg::LightAmbient3::registerInfo.textureCount,
        };
        m_pScene->registerLight(lightTypeInfo, registerInfo);
        m_pScene->addLight(m_pAmbientLight.get());
    }
}

void Window::_enableLighting()
{
    m_pRenderer->enableLighting();
}

void Window::_enableShadow()
{
    m_pRenderer->enableShadow();
}

void Window::_updateLights()
{
    auto position = vg::Vector3(0.0f, 50.0f, 25.0f);

    position.x = cos(glm::radians(m_passedTime * 360.0f)) * 40.0f + position.x;
    position.y = - sin(glm::radians(m_passedTime * 360.0f)) * 20.0f + position.y;
    position.z = sin(glm::radians(m_passedTime * 360.0f)) * 5.0f + position.z;

    // auto rotationMatrix = glm::toMat4(glm::rotation(vg::Vector3(0.0f, 0.0f, 1.0f), glm::normalize(-position)));
    // auto translateMatrix = glm::translate(glm::mat4(1.0f), position);
    // auto lookAtMatrix = translateMatrix * rotationMatrix;
    m_pSpotLight->getTransform()->lookAt2(position, vg::Vector3(0.0f), vg::Vector3(0.0, 1.0, 0.0));
}

void Window::_onPostReCreateSwapchain()
{
    _enableLighting();
    _enableShadow();
}

void Window::_onUpdate()
{
    ParentWindowType::_onUpdate();

    _updateLights();

#ifdef USE_IMGUI_BIND
    auto pos = m_lastWinPos;
    auto size = m_lastWinSize;
    ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y + size.y + 10));
    ImGui::SetNextWindowSize(ImVec2(0, 0));
    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    if (ImGui::SliderFloat("Light Range", &m_lightRange, MIN_LIGHT_RANGE, MAX_LIGHT_RANGE)) {
        m_pSpotLight->setRange(m_lightRange);
    }
    if (ImGui::ColorPicker4("Light Strength", reinterpret_cast<float *>(&m_lightStrength))) {
        m_pSpotLight->setStrength(m_lightStrength);
    }
    if (ImGui::ColorPicker4("Ambient Light Strength", reinterpret_cast<float *>(&m_ambientLightStrength))) {
        m_pAmbientLight->setStrength(m_ambientLightStrength);
    }
    ImGui::End();
#endif //USE_IMGUI_BIND
}