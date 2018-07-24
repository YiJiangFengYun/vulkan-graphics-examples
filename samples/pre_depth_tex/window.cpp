#include "pre_depth_tex/window.hpp"

#include <iostream>
#include <gli/gli.hpp>

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
{
    _init();
}

void Window::_init()
{
    ParentWindowType::_init();
    _createModel();
    _createMaterial();
    _initScene();
    _enablePreDepth();
}

void Window::_initState()
{
    ParentWindowType::_initState();
    m_cameraZoom = -15.0f;
    /// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
    m_cameraRotation = vg::Vector3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
    m_otherInfo.zNear = 0.01f;
    m_otherInfo.zFar = 100.0f;
    m_preDepthScene = VG_TRUE;
}

void Window::_createModel()
{
    const uint32_t layoutCount = 4u;
    sampleslib::AssimpScene::VertexLayoutComponent layouts[layoutCount] = {
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_POSITION,
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_NORMAL,
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_UV,
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_COLOR                        
    };
    sampleslib::AssimpScene::CreateInfo createInfo;
    createInfo.fileName = "models/voyager/voyager.dae";
    createInfo.isCreateObject = VG_TRUE;
    createInfo.layoutComponentCount = layoutCount;
    createInfo.pLayoutComponent = layouts;
    createInfo.offset = vg::Vector3(0.0f, 0.0f, 0.0f);
    m_assimpScene.init(createInfo);
}

void Window::_createMaterial()
{
    auto & pApp = vg::pApp;
    {
        //material
        auto & pMaterial = m_pMaterial;
        vg::Material::MaterialCreateInfo materialCreateInfo = {
            VG_FALSE,
            VG_TRUE,
        };
        pMaterial = std::shared_ptr<vg::Material>(new vg::Material(materialCreateInfo));
        pMaterial->setRenderPriority(0u);
        pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);

        auto pShader = pMaterial->getMainShader();
        auto pPass = pMaterial->getMainPass();
        auto pPreDepthPass = pMaterial->getPreDepthPass();
        
        //shader
        pShader->load("shaders/pre_depth_tex/pre_depth_tex_show.vert.spv",
            "shaders/pre_depth_tex/pre_depth_tex_show.frag.spv");

        pPass->setCullMode(vk::CullModeFlagBits::eBack);
        pPass->setFrontFace(vk::FrontFace::eCounterClockwise);

        vg::Pass::BuildInDataInfo buildInData;
        buildInData.componentCount = 0u;
        buildInData.pComponent = nullptr;
        pPass->setBuildInDataInfo(buildInData);

        vg::PassDataInfo otherDataInfo = {
            VG_PASS_OTHER_DATA_MIN_LAYOUT_PRIORITY,
            vk::ShaderStageFlagBits::eFragment,
        };

        pPass->addData("other_info", otherDataInfo, m_otherInfo);


        pPreDepthPass->setCullMode(vk::CullModeFlagBits::eBack);
        pPreDepthPass->setFrontFace(vk::FrontFace::eClockwise);

        vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
        depthStencilState.depthTestEnable = VG_TRUE;
        depthStencilState.depthWriteEnable = VG_TRUE;
        depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
        pPreDepthPass->setDepthStencilInfo(depthStencilState);
        
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
}

void Window::_enablePreDepth()
{
    m_pRenderer->enablePreDepth();
}

void Window::_onUpdate()
{
    ParentWindowType::_onUpdate();

    if (m_otherInfo.zNear != m_pCamera->getDepthNear() ||
        m_otherInfo.zFar != m_pCamera->getDepthFar()) 
    {
        m_otherInfo.zNear = m_pCamera->getDepthNear();
        m_otherInfo.zFar = m_pCamera->getDepthFar();
        m_pMaterial->getMainPass()->setData("other_info", m_otherInfo);
        m_pMaterial->apply();
    }
}