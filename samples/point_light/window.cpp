#include "point_light/window.hpp"

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
    , m_pBoxMesh()
    , m_pBoxObj()
    , m_pMaterial()
    , m_pBoxMaterial()
    , m_pPointLight()
    , m_lightY(DEFAULT_LIGHT_Y)
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
    , m_pBoxMesh()
    , m_pBoxObj()
    , m_pMaterial()
    , m_pBoxMaterial()
    , m_pPointLight()
    , m_lightY(DEFAULT_LIGHT_Y)
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
    m_cameraZoom = -10.0f;
    /// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
    m_cameraRotation = vg::Vector3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
    m_cameraPosition = vg::Vector3(0.0f, 20.0f, 20.0f);
    m_lightRange = DEFAULT_LIGHT_RANGE;
}

void Window::_createModel()
{
    {
        const uint32_t layoutCount = 4u;
        sampleslib::AssimpScene::VertexLayoutComponent layouts[layoutCount] = {
            sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_POSITION,
            sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_UV,
            sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_COLOR,
            sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_NORMAL,
        };
        sampleslib::AssimpScene::CreateInfo createInfo;
        createInfo.fileName = "models/shadowscene_fire.dae";
        createInfo.isCreateObject = VG_TRUE;
        createInfo.layoutComponentCount = layoutCount;
        createInfo.pLayoutComponent = layouts;
        createInfo.offset = vg::Vector3(0.0f, 0.0f, 0.0f);
        createInfo.scale = vg::Vector3(1.0f);
        m_assimpScene.init(createInfo);

        const auto &objects = m_assimpScene.getObjects();
        for (const auto &object : objects)
        {
            vg::Matrix4x4 matrix = vg::Matrix4x4(1.0f);
            matrix = glm::rotate(matrix, glm::radians(120.0f), vg::Vector3(0.0f, 1.0f, 0.0f));
            object->getTransform()->setLocalMatrix(matrix);
        }
    }
    
    // box
    {
        std::vector<vg::Vector3> tempPositions = { 
            { -1.0f, -1.0f,  1.0f }, 
            {  1.0f, -1.0f,  1.0f }, 
            {  1.0f,  1.0f,  1.0f }, 
            { -1.0f,  1.0f,  1.0f }, 
            { -1.0f, -1.0f, -1.0f }, 
            {  1.0f, -1.0f, -1.0f },
            {  1.0f,  1.0f, -1.0f },
            { -1.0f,  1.0f, -1.0f },
        };
    
        std::vector<vg::Vector4> tempColors = {
            { 1.0f, 0.0f, 0.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f, 1.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
            { 1.0f, 0.0f, 0.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f, 1.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };
        
        std::vector<uint32_t> tempIndices = {
            0,1,2, 2,3,0, 1,5,6, 6,2,1, 7,6,5, 5,4,7, 4,0,3, 3,7,4, 4,5,1, 1,0,4, 3,2,6, 6,7,3,
        };

        auto pMesh = static_cast<std::shared_ptr<vg::DimSepMesh3>>(new vg::DimSepMesh3());
        pMesh->setVertexCount(static_cast<uint32_t>(tempPositions.size()));
        pMesh->addPositions(tempPositions);
        pMesh->addColors(tempColors);
        pMesh->setIndices(tempIndices, vg::PrimitiveTopology::TRIANGLE_LIST, 0u);
        pMesh->apply(VG_TRUE);
        m_pBoxMesh = pMesh;

        auto obj = std::shared_ptr<vg::VisualObject3>(new vg::VisualObject3());
        obj->setMesh(pMesh.get());
        m_pBoxObj = obj;
    }
}

void Window::_createLights()
{
    m_pPointLight = std::shared_ptr<vg::LightPoint3>{new vg::LightPoint3(m_lightRange
        , 2048, 2048)};
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
        pShader->load("shaders/point_light/scene.vert.spv",
            "shaders/point_light/scene.frag.spv");
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

    {
        //material
        auto & pMaterial = m_pBoxMaterial;
        pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
        pMaterial->setRenderPriority(0u);
        pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);
        auto pShader = pMaterial->getMainShader();
        auto pPass = pMaterial->getMainPass();
        
        //shader
        pShader->load("shaders/point_light/cube.vert.spv",
            "shaders/point_light/cube.frag.spv");
        //pass
        const uint32_t componentCount = 1u;
        vg::Pass::BuildInDataInfo::Component buildInDataCmps[componentCount] = {
            {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
        };
        vg::Pass::BuildInDataInfo buildInDataInfo;
        buildInDataInfo.componentCount = componentCount;
        buildInDataInfo.pComponent = buildInDataCmps;
        pPass->setBuildInDataInfo(buildInDataInfo);
        pPass->setCullMode(vk::CullModeFlagBits::eBack);
        pPass->setFrontFace(vk::FrontFace::eClockwise);

        vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
        depthStencilState.depthTestEnable = VG_TRUE;
        depthStencilState.depthWriteEnable = VG_TRUE;
        depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
        pPass->setDepthStencilInfo(depthStencilState);

        auto pCubeTex = m_pPointLight->getLightDistTargetCube()->getColorTargetTexture();

        vg::PassTextureInfo::TextureInfo itemInfo = {
            pCubeTex,
            nullptr,
            pCubeTex->getSampler(),
            vk::ImageLayout::eUndefined,
        };
        vg::PassTextureInfo mainTextureInfo = {
            vg::SamplerTextureType::TEX_CUBE,
            1u,
            &itemInfo,
            VG_PASS_OTHER_MIN_BINDING_PRIORITY,
            vg::ImageDescriptorType::COMBINED_IMAGE_SAMPLER,
            vk::ShaderStageFlagBits::eFragment,
        };
        pPass->addTexture("main_texture", mainTextureInfo);
        pPass->apply();
        
        pMaterial->apply();
    }

}

void Window::_initScene()
{
    // {
    //     auto pObj = m_pBoxObj;
    //     pObj->setMaterialCount(1u);
    //     pObj->setMaterial(m_pBoxMaterial.get());
    //     pObj->setLightingMaterial(typeid(vg::LightPoint3), vg::pDefaultLightingPointDistMaterial.get());
    //     m_pScene->addVisualObject(pObj.get());
        
    // }
    {
        const auto &objects = m_assimpScene.getObjects();
        for (const auto &object : objects)
        {
            object->setMaterialCount(1u);
            object->setMaterial(m_pMaterial.get());
            object->setLightingMaterial(typeid(vg::LightPoint3), vg::pDefaultLightingPointDistMaterial.get());
            m_pScene->addVisualObject(object.get());
        }
    }

    {
        const auto &light_type_info = typeid(vg::LightPoint3);
        vg::SceneLightRegisterInfo registerInfo = {
            0u,
            MAX_LIGHT_COUNT,
            vg::LightPoint3::registerInfo.dataSize,
            vg::LightPoint3::registerInfo.textureCount,
        };
        m_pScene->registerLight(light_type_info, registerInfo);
        m_pScene->addLight(m_pPointLight.get());
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
    // auto position = vg::Vector3(0.0f, 25.0f, 10.0f);

    // position.x = cos(glm::radians(m_passedTime * 360.0f)) * 40.0f + position.x;
    // position.y = - sin(glm::radians(m_passedTime * 360.0f)) * 20.0f + position.y;
    // position.z = sin(glm::radians(m_passedTime * 360.0f)) * 5.0f + position.z;

    vg::Vector3 position = vg::Vector3(0.0f);
    position.y = m_lightY;
    position.x = sin(glm::radians(m_passedTime * 360.0f)) * 0.3f;
	position.z = cos(glm::radians(m_passedTime * 360.0f)) * 0.3f;

    m_pPointLight->getTransform()->lookAt2(position, vg::Vector3(0.0f), vg::Vector3(0.0, 1.0, 0.0));
    // m_pPointLight->getTransform()->setLocalPosition(position);
}

void Window::_onPostReCreateSwapchain()
{
    _enableLighting();
    _enableShadow();
}

void Window::_onUpdate()
{
    ParentWindowType::_onUpdate();

    {
        vg::Matrix4x4 transform(1.0f);
        transform = glm::translate(transform, vg::Vector3(-10.0f, 20.0f, 0.0f));
        //transform = glm::rotate(transform, m_passedTime * glm::radians(360.0f), vg::Vector3(1.0f, 0.0f, 0.0f));
        //transform = glm::rotate(transform, m_passedTime * glm::radians(360.0f), vg::Vector3(0.0f, 1.0f, 0.0f));
        transform = glm::scale(transform, vg::Vector3(3.0f));
        m_pBoxObj->getTransform()->setLocalMatrix(transform);
        
    }

    _updateLights();

#ifdef USE_IMGUI_BIND
    auto pos = m_lastWinPos;
    auto size = m_lastWinSize;
    ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y + size.y + 10));
    ImGui::SetNextWindowSize(ImVec2(0, 0));
    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    if (ImGui::SliderFloat("Range", &m_lightRange, MIN_LIGHT_RANGE, MAX_LIGHT_RANGE)) {
        m_pPointLight->setRange(m_lightRange);
    }
    if (ImGui::SliderFloat("LightY", &m_lightY, MIN_LIGHT_Y, MAX_LIGHT_Y)) {
    }
    ImGui::End();
#endif //USE_IMGUI_BIND
}