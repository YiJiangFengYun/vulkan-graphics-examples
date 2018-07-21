#include "spot_light/window.hpp"

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
    , m_pMaterial()
    , m_pSpotLight()
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

    glfwSetKeyCallback(m_pWindow.get(), [](GLFWwindow *glfwWindow, int key, int scancode, int action, int mods)
    {
        Window* const instance = (Window*)glfwGetWindowUserPointer(glfwWindow);

        if (action == GLFW_PRESS && key == GLFW_KEY_SPACE)
        {
            instance->m_isPause = ! instance->m_isPause;
        }
    });
}

void Window::_initState()
{
    ParentWindowType::_initState();
    m_cameraZoom = -15.0f;
    /// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
    m_cameraRotation = vg::Vector3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
    m_timerSpeedFactor = 0.1f;
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
    m_pSpotLight = std::shared_ptr<vg::LightSpot3>{ new vg::LightSpot3(glm::radians(45.0f), 100.0f, 2048, 2048, vk::Format::eD16Unorm)};

    _updateLights();
}

void Window::_createMaterial()
{
    auto & pApp = vg::pApp;
    {
        //material
        vg::Material::MaterialCreateInfo createInfo = {
            VG_FALSE,
            VG_TRUE,
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
        vg::Pass::BuildInDataInfo::Component buildInDataCmps[2] = {
                {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
                {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD},
            };
        vg::Pass::BuildInDataInfo buildInDataInfo;
        buildInDataInfo.componentCount = 2u;
        buildInDataInfo.pComponent = buildInDataCmps;
        pPass->setBuildInDataInfo(buildInDataInfo);
        pPass->setCullMode(vk::CullModeFlagBits::eBack);
        pPass->setFrontFace(vk::FrontFace::eClockwise);
        vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
        depthStencilState.depthTestEnable = VG_TRUE;
        depthStencilState.depthWriteEnable = VG_TRUE;
        depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
        pPass->setDepthStencilInfo(depthStencilState);

        vg::PassDataInfo otherDataInfo = {
            VG_PASS_OTHER_DATA_MIN_LAYOUT_PRIORITY,
            vk::ShaderStageFlagBits::eVertex,
        };
        //view poss
        pPass->addData("other_data", otherDataInfo, m_pCamera->getTransform()->getPosition());
        pPass->apply();

        auto pPreDepthPass = pMaterial->getPreDepthPass();
        pPreDepthPass->setFrontFace(vk::FrontFace::eClockwise);
        pPreDepthPass->setCullMode(vk::CullModeFlagBits::eBack);

        depthStencilState.depthTestEnable = VG_TRUE;
        depthStencilState.depthWriteEnable = VG_TRUE;
        depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
        pPreDepthPass->setDepthStencilInfo(depthStencilState);

        //depth bias
        vg::Pass::DepthBiasInfo depthBiasInfo = {
            VG_TRUE,
            VG_FALSE,
            1.25f,
            0.0f,
            1.75f,
        };
        pPreDepthPass->setDepthBiasInfo(depthBiasInfo);

        pPreDepthPass->apply();
        
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
        m_pScene->addVisualObject(object.get());
    }

    {
        const auto &light_type_info = typeid(vg::LightSpot3);
        vg::SceneLightRegisterInfo registerInfo = {
            0u,
            MAX_LIGHT_COUNT,
            vg::LightSpot3::registerInfo.dataSize,
            vg::LightSpot3::registerInfo.textureCount,
        };
        m_pScene->registerLight(light_type_info, registerInfo);
        m_pScene->addLight(m_pSpotLight.get());
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

void Window::_onUpdate()
{
    ParentWindowType::_onUpdate();

    _updateLights();

    auto & pMaterial = m_pMaterial;
    auto pPass = pMaterial->getMainPass();
    pPass->setData("other_data", m_pCamera->getTransform()->getPosition());
    pPass->apply();
}