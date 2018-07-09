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
}

void Window::_initState()
{
    ParentWindowType::_initState();
    m_cameraZoom = -15.0f;
    /// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
    m_cameraRotation = vg::Vector3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
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
    m_assimpScene.init(createInfo);
}

void Window::_createLights()
{
    m_pSpotLight = std::shared_ptr<vg::LightSpot3>{ new vg::LightSpot3()};
    m_pSpotLight->getTransform()->setLocalPosition(vg::Vector3(25.0f, 5.0f, 5.0f));
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
        pShader->load("shaders/point_light/scene.vert.spv",
            "shaders/point_light/scene.frag.spv");
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
            vg::LightPoint3::registerInfo.dataSize,
            vg::LightPoint3::registerInfo.textureCount,
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

void Window::_onUpdate()
{
    ParentWindowType::_onUpdate();

    auto & pMaterial = m_pMaterial;
    auto pPass = pMaterial->getMainPass();
    pPass->setData("other_data", m_pCamera->getTransform()->getPosition());
    pPass->apply();
}