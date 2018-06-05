#include "off_screen/window.hpp"

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
{
    _init();
    
}

Window::Window(std::shared_ptr<GLFWwindow> pWindow
    , std::shared_ptr<vk::SurfaceKHR> pSurface
)
    : sampleslib::Window<vg::SpaceType::SPACE_3>(pWindow
        , pSurface
        )
{
    _init();
    
}

void Window::_init()
{
    ParentWindowType::_init();

    _createModel();
    _createSceneOffScreen();
    _createOffscreenTex();
    _createOffscreenRenderer();
    _createTexture();
    _createMaterial();
    _createVisualObjects();
    _initScene();
}

void Window::_initState()
{
    ParentWindowType::_initState();
    m_cameraPosition = { 0.0f, 4.0f, -4.0f };
    /// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
    m_cameraRotation = vg::Vector3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));

    m_otherInfoOffScreen.matrixInverse = glm::scale(vg::Matrix4x4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f));
    m_otherInfo.lightPos = m_otherInfoOffScreen.lightPos = vg::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Window::_createModel()
{
    const uint32_t layoutCount = 4u;
    sampleslib::AssimpScene::VertexLayoutComponent layouts[layoutCount] = {
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_POSITION,
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_UV,
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_COLOR,
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_NORMAL                                
    };
    sampleslib::AssimpScene::CreateInfo createInfo;
    createInfo.fileName = "models/chinesedragon.dae";
    createInfo.isCreateObject = VG_FALSE;
    createInfo.layoutComponentCount = layoutCount;
    createInfo.pLayoutComponent = layouts;
    createInfo.offset = vg::Vector3(0.0f, 1.5f, 0.0f);
    createInfo.scale = vg::Vector3(0.3f);
    m_assimpSceneModel.init(createInfo);

    createInfo.fileName = "models/plane.obj";
    createInfo.isCreateObject = VG_TRUE;
    createInfo.offset = vg::Vector3(0.0f, 0.0f, 0.0f);
    createInfo.scale = vg::Vector3(0.5f);

    m_assimpScenePlane.init(createInfo);
}

void Window::_createSceneOffScreen()
{
    m_pSceneOffScreen = std::shared_ptr<vg::Scene3>(new vg::Scene3());
    m_pCameraOffScreen = std::shared_ptr<vg::Camera3>(new vg::Camera3());
    m_pCameraOffScreen->updateProj(m_pCamera->getFovY(),
        m_pCamera->getAspect(),
        m_pCamera->getZNear(),
        m_pCamera->getZFar());
    m_pSceneOffScreen->addCamera(m_pCameraOffScreen.get());
}

void Window::_createOffscreenTex()
{
    m_pOffScreenTex = std::shared_ptr<vg::Texture2DColorAttachment>{
        new vg::Texture2DColorAttachment(FB_COLOR_FORMAT, FB_DIM, FB_DIM),
    };
}

void Window::_createOffscreenRenderer()
{
    m_pOffScreenRenderTarget = std::shared_ptr<vg::ColorTexRenderTarget>{
        new vg::ColorTexRenderTarget(m_pOffScreenTex.get()),
    };
    m_pOffScreenRenderer = std::shared_ptr<vg::Renderer>{
        new vg::Renderer{
            m_pOffScreenRenderTarget.get(),
        } 
    };
}

void Window::_createTexture()
{
    //load texture
    auto &pApp = vg::pApp;
    auto deviceFeatures = pApp->getPhysicalDeviceFeatures();
    std::string fileName;
    vk::Format format;
    if (deviceFeatures.textureCompressionBC) 
    {
        fileName = "textures/darkmetal_bc3_unorm.ktx";
        format = vk::Format::eBc3UnormBlock;
    }
    else if (deviceFeatures.textureCompressionASTC_LDR)
    {
        fileName = "textures/darkmetal_astc_8x8_unorm.ktx";
        format = vk::Format::eAstc8x8UnormBlock;
    }
    else if (deviceFeatures.textureCompressionETC2)
    {
        fileName = "textures/darkmetal_etc2_unorm.ktx";
        format = vk::Format::eEtc2R8G8B8UnormBlock;
    }
    else
    {
        throw std::runtime_error("Device does not support any compressed texture format!");
    }

    gli::texture2d gliTex(gli::load(fileName));
    if (gliTex.empty()) {
        throw std::runtime_error("The texture do't exist! path: " + fileName);
    }

    auto pTex = new vg::Texture2D(format, VG_TRUE,
        gliTex[0].extent().x,
        gliTex[0].extent().y
    );
    m_pTexturePlane = std::shared_ptr<vg::Texture2D>(pTex);
    uint32_t mipLevels = static_cast<uint32_t>(gliTex.levels());
    uint32_t count = mipLevels;
    vg::TextureDataInfo textureLayout;
    std::vector<vg::TextureDataInfo::Component> components(count);
    for (uint32_t level = 0; level < mipLevels; ++level) {
        uint32_t index = level;
        components[index].mipLevel = level;
        components[index].baseArrayLayer = 0u;
        components[index].layerCount = 1u;
        components[index].size = gliTex[level].size();
        components[index].hasImageExtent = VG_TRUE;
        components[index].width = gliTex[level].extent().x;
        components[index].height = gliTex[level].extent().y;
        components[index].depth = 1u;
    }
    textureLayout.componentCount = components.size();
    textureLayout.pComponent = components.data();
    m_pTexturePlane->applyData(textureLayout, gliTex.data(), gliTex.size());

    auto pDevice = pApp->getDevice();
    auto pPhysicalDevice = pApp->getPhysicalDevice();
    vk::Bool32 enableAnisotropy = VK_FALSE;
    float anisotropy = 0.0f;
    if (pApp->getPhysicalDeviceFeatures().samplerAnisotropy)
    {
        enableAnisotropy = VK_TRUE;
        anisotropy = pPhysicalDevice->getProperties().limits.maxSamplerAnisotropy;
    }

    vg::Texture::SamplerCreateInfo info = {
        vk::SamplerCreateFlags(),
        vk::Filter::eLinear,
        vk::Filter::eLinear,
        vk::SamplerMipmapMode::eLinear,
        vk::SamplerAddressMode::eRepeat,
        vk::SamplerAddressMode::eRepeat,
        vk::SamplerAddressMode::eRepeat,
        0.0f,
        enableAnisotropy,
        anisotropy,
        0.0f,
        (float)(m_pTexturePlane->getImage()->getInfo().mipLevels),
    };
    m_pTexturePlane->createSampler("other_sampler", info);
}

void Window::_createMaterial()
{
    auto & pApp = vg::pApp;
    {
        auto &pMaterial = m_pMaterialModel;
        //material
        pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
        pMaterial->setRenderPriority(0u);
        pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);

        auto pShader = pMaterial->getMainShader();
        auto pPass = pMaterial->getMainPass();
        //shader
        pShader->load("shaders/off_screen/phong.vert.spv", "shaders/off_screen/phong.frag.spv");
        //pass
        vg::Pass::BuildInDataInfo::Component buildInDataCmps[2] = {
                {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
                {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_VIEW}
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
        pPass->addData("other_info", otherDataInfo, m_otherInfo);
        pPass->apply();
        
        pMaterial->apply();
    }

    {
        auto & pMaterial = m_pMaterialModelOffscreen;
        //material
        pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
        pMaterial->setRenderPriority(0u);
        pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);

        auto pShader = pMaterial->getMainShader();
        auto pPass = pMaterial->getMainPass();
        
        //shader
        pShader->load("shaders/off_screen/phong_offscreen.vert.spv", "shaders/off_screen/phong_offscreen.frag.spv");
        //pass
        vg::Pass::BuildInDataInfo::Component buildInDataCmps[3] = {
                {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD},
                {vg::Pass::BuildInDataType::MATRIX_VIEW},
                {vg::Pass::BuildInDataType::MATRIX_PROJECTION},
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
        vg::PassDataInfo otherDataInfo = {
            VG_PASS_OTHER_DATA_MIN_LAYOUT_PRIORITY,
            vk::ShaderStageFlagBits::eVertex,
        };
        pPass->addData("other_info", otherDataInfo, m_otherInfoOffScreen);
        pPass->apply();
        
        pMaterial->apply();
    }

    {
        auto & pMaterial = m_pMaterialPlane;
        //material
        pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
        pMaterial->setRenderPriority(0u);
        pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);

        auto pShader = pMaterial->getMainShader();
        auto pPass = pMaterial->getMainPass();
        

        //shader
        pShader->load("shaders/off_screen/mirror.vert.spv", "shaders/off_screen/mirror.frag.spv");
        
        vg::Pass::BuildInDataInfo::Component buildInDataCmps[3] = {
                {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
            };
            vg::Pass::BuildInDataInfo buildInDataInfo;
            buildInDataInfo.componentCount = 3u;
            buildInDataInfo.pComponent = buildInDataCmps;
            pPass->setBuildInDataInfo(buildInDataInfo);
        pPass->setCullMode(vk::CullModeFlagBits::eNone);
        pPass->setFrontFace(vk::FrontFace::eClockwise);
        vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
        depthStencilState.depthTestEnable = VG_TRUE;
        depthStencilState.depthWriteEnable = VG_TRUE;
        depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
        pPass->setDepthStencilInfo(depthStencilState);
        vg::PassTextureInfo::TextureInfo mainTextureItemInfo = {
            m_pTexturePlane.get(),
            nullptr,
            m_pTexturePlane->getSampler("other_sampler"),
            vk::ImageLayout::eUndefined,
        };
        vg::PassTextureInfo mainTextureInfo = {
            1u,
            &mainTextureItemInfo,
            VG_PASS_OTHER_MIN_BINDING_PRIORITY,
            vg::ImageDescriptorType::COMBINED_IMAGE_SAMPLER,
            vk::ShaderStageFlagBits::eFragment,
        };
        pPass->addTexture("main_texture", mainTextureInfo);
        vg::PassTextureInfo::TextureInfo offscreenTextureItemInfo = {
            m_pOffScreenTex.get(),
            nullptr,
            nullptr,
            vk::ImageLayout::eUndefined,
        };
        vg::PassTextureInfo offscreenTextureInfo = {
            1u,
            &offscreenTextureItemInfo,
            VG_PASS_OTHER_MIN_BINDING_PRIORITY + 1,
            vg::ImageDescriptorType::COMBINED_IMAGE_SAMPLER,
            vk::ShaderStageFlagBits::eFragment,
        };
        pPass->addTexture("offcreen_texture", offscreenTextureInfo);
        pPass->apply();
        
        pMaterial->apply();
       
    }

}

void Window::_createVisualObjects()
{
    const auto &meshes = m_assimpSceneModel.getMeshes();    
    m_pVisualObjects.resize(meshes.size());
    m_pVisualObjectOffscreens.resize(meshes.size());
    uint32_t index = 0;
    uint32_t count = static_cast<uint32_t>(meshes.size());
    for (index = 0; index < count; ++index)
    {
        auto &mesh = meshes[index];
        m_pVisualObjects[index] = std::shared_ptr<vg::VisualObject3>{ new vg::VisualObject3() };
        m_pVisualObjects[index]->setMesh(mesh.get());
        auto pMaterial = m_pMaterialModel.get();
        m_pVisualObjects[index]->setMaterialCount(1u);
        m_pVisualObjects[index]->setMaterial(pMaterial);

        m_pVisualObjectOffscreens[index] = std::shared_ptr<vg::VisualObject3>{ new vg::VisualObject3() };
        m_pVisualObjectOffscreens[index]->setMesh(mesh.get());
        pMaterial = m_pMaterialModelOffscreen.get();
        m_pVisualObjectOffscreens[index]->setMaterialCount(1U);
        m_pVisualObjectOffscreens[index]->setMaterial(pMaterial);
        m_pVisualObjectOffscreens[index]->setIsVisibilityCheck(VG_FALSE);
    }
}

void Window::_initScene()
{
    {
        const auto &objects = m_pVisualObjects;
        for (const auto &object : objects)
        {
            m_pScene->addVisualObject(object.get());    
        }
    }

    {
        const auto &objects = m_pVisualObjectOffscreens;
        for (const auto &object : objects)
        {
            m_pSceneOffScreen->addVisualObject(object.get());
        }
    }

    {
        const auto &objects = m_assimpScenePlane.getObjects();
        for (const auto &object : objects)
        {
            auto pMaterial = m_pMaterialPlane.get();
            object->setMaterialCount(1u);
            object->setMaterial(pMaterial);
            m_pScene->addVisualObject(object.get());
            // object->setIsVisibilityCheck(VG_FALSE);    
        }
    }
}

void Window::_onUpdate()
{
    ParentWindowType::_onUpdate();

    auto cameraMatrix = m_pCamera->getTransform()->getLocalMatrix();
    m_pCameraOffScreen->getTransform()->setLocalMatrix(cameraMatrix);
#ifdef USE_IMGUI_BIND
    // auto pos = m_lastWinPos;
    // auto size = m_lastWinSize;
    // ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y + size.y + 10));
    // ImGui::SetNextWindowSize(ImVec2(0, 0));
    // ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    // ImGui::End();
#endif //USE_IMGUI_BIND
}

void Window::_doRender(vg::Renderer::RenderInfo &info
            , vg::Renderer::RenderResultInfo &resultInfo)
{
    uint32_t drawCount = 0u;

    vg::Renderer::RenderInfo infoOffscreen;
    vg::Renderer::SceneInfo tempSceneInfo;
    tempSceneInfo.pCamera = m_pCameraOffScreen.get();
    tempSceneInfo.pScene = m_pSceneOffScreen.get();
    infoOffscreen.sceneInfoCount = 1u;
    infoOffscreen.pSceneInfos = &tempSceneInfo;
    vg::Renderer::RenderResultInfo offscreenResultInfo;
    

    m_pOffScreenRenderer->render(infoOffscreen, offscreenResultInfo);
    drawCount += offscreenResultInfo.drawCount;


    vg::Renderer::RenderResultInfo tempResultInfo;

    ParentWindowType::_doRender(info, tempResultInfo);

    drawCount += tempResultInfo.drawCount;
    resultInfo = tempResultInfo;
    resultInfo.drawCount = drawCount;
}