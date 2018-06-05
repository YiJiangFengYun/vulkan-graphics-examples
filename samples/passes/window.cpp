#include "passes/window.hpp"

#include <iostream>
#include <random>
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
    _initLights();
    _createModel();
    _createTexture();
    _createMaterial();
    _initScene();
}

void Window::_initState()
{
    ParentWindowType::_initState();
    m_cameraZoom = -0.0f;
    m_cameraPosition = vg::Vector3(0.0f, 2.0f, -2.0f);
    /// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
    m_cameraRotation = vg::Vector3(glm::radians(-30.0f), glm::radians(0.0f), glm::radians(0.0f));
}

void Window::_initLights()
{
    std::vector<vg::Vector3> colors =
    {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
    };

    std::default_random_engine rndGen((unsigned)time(nullptr));
    std::uniform_real_distribution<float> rndDist(-1.0f, 1.0f);
    std::uniform_int_distribution<uint32_t> rndCol(0, static_cast<uint32_t>(colors.size()-1));

    for (auto& light : m_otherInfo.lights)
    {
        light.position = glm::vec4(rndDist(rndGen) * 6.0f, 0.25f + std::abs(rndDist(rndGen)) * 4.0f, rndDist(rndGen) * 6.0f, 1.0f);
        light.color = colors[rndCol(rndGen)];
        light.radius = 1.0f + std::abs(rndDist(rndGen));            
    }
}

void Window::_createModel()
{
    const uint32_t layoutCount = 4u;
    sampleslib::AssimpScene::VertexLayoutComponent layouts[layoutCount] = {
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_POSITION,
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_COLOR,        
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_NORMAL,
        sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_UV,
    };
    sampleslib::AssimpScene::CreateInfo createInfo;
    createInfo.fileName = "models/samplebuilding.dae";
    createInfo.isCreateObject = VG_TRUE;
    createInfo.layoutComponentCount = layoutCount - 1;
    createInfo.pLayoutComponent = layouts;
    createInfo.offset = vg::Vector3(0.0f, 0.0f, 0.0f);
    m_assimpScene.init(createInfo);

    createInfo.fileName = "models/samplebuilding_glass.dae";
    createInfo.layoutComponentCount = layoutCount; //add uv component.
    m_assimpSceneGlass.init(createInfo);
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
        fileName = "textures/colored_glass_bc3_unorm.ktx";
        format = vk::Format::eBc3UnormBlock;
    }
    else if (deviceFeatures.textureCompressionASTC_LDR)
    {
        fileName = "textures/colored_glass_astc_8x8_unorm.ktx";
        format = vk::Format::eAstc8x8UnormBlock;
    }
    else if (deviceFeatures.textureCompressionETC2)
    {
        fileName = "textures/colored_glass_etc2_unorm.ktx";
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
        gliTex[0].extent().y,
        vk::ImageUsageFlags(),
        VG_TRUE,
        VG_FALSE
    );
    m_pTextureOfSceneGlass = std::shared_ptr<vg::Texture2D>(pTex);
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
    m_pTextureOfSceneGlass->applyData(textureLayout, gliTex.data(), gliTex.size());

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
        (float)(m_pTextureOfSceneGlass->getImage()->getInfo().mipLevels),
        vk::BorderColor::eFloatTransparentBlack,
    };
    m_pSamplerOfSceneGlass = m_pTextureOfSceneGlass->createSampler("other_sampler", info);
}

void Window::_createMaterial()
{
    {
        //material
        uint32_t deferredAttachmentInfoCount = 3u;
        std::vector<vge::MaterialDeferred::DeferredAttachmentInfo> deferredAttachmentInfos(deferredAttachmentInfoCount);
        deferredAttachmentInfos[0].format = vk::Format::eR16G16B16A16Sfloat;
        deferredAttachmentInfos[1].format = vk::Format::eR16G16B16A16Sfloat;
        deferredAttachmentInfos[2].format = vk::Format::eR8G8B8A8Unorm;
        vge::MaterialDeferred::CreateInfo createInfo = {
            m_width,
            m_height,
            vk::Format::eR8G8B8A8Unorm,
            vk::Format::eD32SfloatS8Uint,
            deferredAttachmentInfoCount,
            deferredAttachmentInfos.data(),
        };
        m_pMaterialOfScene = std::shared_ptr<vge::MaterialDeferred>(new vge::MaterialDeferred(createInfo));
        m_pMaterialOfScene->setRenderPriority(0u);
        m_pMaterialOfScene->setRenderQueueType(vg::MaterialShowType::OPAQUE);
        {
            auto pPass = m_pMaterialOfScene->getPassDeferred();
            auto pShader = pPass->getShader();

            //shader
            pShader->load("shaders/passes/gbuffer.vert.spv", 
                "shaders/passes/gbuffer.frag.spv");

            //pass
            pPass->setPolygonMode(vk::PolygonMode::eFill);
            pPass->setCullMode(vk::CullModeFlagBits::eBack);
            pPass->setFrontFace(vk::FrontFace::eClockwise);
            vg::Pass::BuildInDataInfo::Component buildInDataCmps[2] = {
                {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
                {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD}
            };
            vg::Pass::BuildInDataInfo buildInDataInfo;
            buildInDataInfo.componentCount = 2u;
            buildInDataInfo.pComponent = buildInDataCmps;
            pPass->setBuildInDataInfo(buildInDataInfo);
        }
        {
            auto pPass = m_pMaterialOfScene->getPassComposition();
            auto pShader = pPass->getShader();
            //shader
            pShader->load("shaders/passes/composition.vert.spv", 
                "shaders/passes/composition.frag.spv");

            uint32_t specializationData = NUM_LIGHTS;

			pPass->addSpecializationItem(vk::ShaderStageFlagBits::eFragment, "num_light", 0u, specializationData);
    
            vg::PassDataInfo otherDataInfo = {
                VG_PASS_OTHER_DATA_MIN_LAYOUT_PRIORITY,
                vk::ShaderStageFlagBits::eFragment,
            };
    
            pPass->addData("other_info", otherDataInfo, m_otherInfo);
        }
        {
            auto pShader = m_pMaterialOfScene->getMainShader();
            auto pPass = m_pMaterialOfScene->getMainPass();
            //shader
            pShader->load("shaders/passes/to_trunk.vert.spv", 
                "shaders/passes/to_trunk.frag.spv");
            //pass
        }
    
        m_pMaterialOfScene->apply();
    }

    {
        m_pMaterialOfSceneTest = std::shared_ptr<vg::Material>{new vg::Material()};
        m_pMaterialOfSceneTest->setRenderPriority(0u);
        m_pMaterialOfSceneTest->setRenderQueueType(vg::MaterialShowType::OPAQUE);
        auto pPass = m_pMaterialOfSceneTest->getMainPass();
        auto pShader = pPass->getShader();
        //shader
        pShader->load("shaders/only_color.vert.spv", 
            "shaders/only_color.frag.spv");
        //pass
        pPass->setPolygonMode(vk::PolygonMode::eFill);
        pPass->setCullMode(vk::CullModeFlagBits::eBack);
        pPass->setFrontFace(vk::FrontFace::eClockwise);
        vg::Pass::BuildInDataInfo::Component buildInDataCmps[2] = {
            {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
            {vg::Pass::BuildInDataType::MAIN_CLOLOR}
        };
        vg::Pass::BuildInDataInfo buildInDataInfo;
        buildInDataInfo.componentCount = 2u;
        buildInDataInfo.pComponent = buildInDataCmps;
        pPass->setBuildInDataInfo(buildInDataInfo);
        vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
        depthStencilState.depthTestEnable = VG_TRUE;
        depthStencilState.depthWriteEnable = VG_TRUE;
        depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
        pPass->setDepthStencilInfo(depthStencilState);

        m_pMaterialOfSceneTest->apply();
    }

    {
        m_pMaterialOfSceneGlass = std::shared_ptr<vg::Material>{new vg::Material()};
        m_pMaterialOfSceneGlass->setRenderPriority(0u);
        m_pMaterialOfSceneGlass->setRenderQueueType(vg::MaterialShowType::TRANSPARENT);
        auto pPass = m_pMaterialOfSceneGlass->getMainPass();
        auto pShader = pPass->getShader();
        //shader
        pShader->load("shaders/passes/transparent.vert.spv", 
            "shaders/passes/transparent.frag.spv");
        //pass
        pPass->setPolygonMode(vk::PolygonMode::eFill);
        pPass->setCullMode(vk::CullModeFlagBits::eNone);
        pPass->setFrontFace(vk::FrontFace::eCounterClockwise);
        vg::Pass::BuildInDataInfo::Component buildInDataCmps[1] = {
            {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
        };
        vg::Pass::BuildInDataInfo buildInDataInfo;
        buildInDataInfo.componentCount = 1u;
        buildInDataInfo.pComponent = buildInDataCmps;
        pPass->setBuildInDataInfo(buildInDataInfo);
        vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
        depthStencilState.depthTestEnable = VG_TRUE;
        depthStencilState.depthWriteEnable = VG_FALSE;
        depthStencilState.depthCompareOp = vk::CompareOp::eLess;
        pPass->setDepthStencilInfo(depthStencilState);

        const uint32_t attachmentCount = 1u;
        vk::PipelineColorBlendAttachmentState attachmentStates[attachmentCount] = {};
        for (uint32_t i = 0; i < attachmentCount; ++i)
        {
            attachmentStates[i].blendEnable = VG_TRUE;            
            attachmentStates[i].colorWriteMask = vk::ColorComponentFlagBits::eR | 
                vk::ColorComponentFlagBits::eG | 
                vk::ColorComponentFlagBits::eB | 
                vk::ColorComponentFlagBits::eA;
            attachmentStates[i].srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
            attachmentStates[i].dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
            attachmentStates[i].colorBlendOp = vk::BlendOp::eAdd;
            attachmentStates[i].srcAlphaBlendFactor = vk::BlendFactor::eOne;
            attachmentStates[i].dstAlphaBlendFactor = vk::BlendFactor::eOne;
            attachmentStates[i].alphaBlendOp = vk::BlendOp::eAdd;
        }
        
        vk::PipelineColorBlendStateCreateInfo colorBlendState = {};
        colorBlendState.attachmentCount = attachmentCount;
        colorBlendState.pAttachments = attachmentStates;
        pPass->setColorBlendInfo(colorBlendState);
        vg::PassTextureInfo::TextureInfo itemInfo = {
            m_pTextureOfSceneGlass.get(),
            nullptr,
            m_pSamplerOfSceneGlass,
            vk::ImageLayout::eUndefined,
        };
        vg::PassTextureInfo mainTextureInfo = {
            1u,
            &itemInfo,
            VG_PASS_OTHER_MIN_BINDING_PRIORITY,
            vg::ImageDescriptorType::COMBINED_IMAGE_SAMPLER,
            vk::ShaderStageFlagBits::eFragment,
        };
        pPass->addTexture("main_texture", mainTextureInfo);

        m_pMaterialOfSceneGlass->apply();
    }
    
}

void Window::_initScene()
{
    {
        const auto &objects = m_assimpScene.getObjects();
        for (const auto &object : objects)
        {
            auto pMaterial = m_pMaterialOfScene.get();
            // auto pMaterial = m_pMaterialOfSceneTest.get();
            object->setMaterialCount(1u);
            object->setMaterial(pMaterial);
            m_pScene->addVisualObject(object.get());        
        }
    }
    {
        const auto &objects = m_assimpSceneGlass.getObjects();
        for (const auto &object : objects)
        {
            auto pMaterial = m_pMaterialOfSceneGlass.get();
            object->setMaterialCount(1u);
            object->setMaterial(pMaterial);
            m_pScene->addVisualObject(object.get());        
        }
    }
    
}

void Window::_onUpdate()
{
    ParentWindowType::_onUpdate();

    m_otherInfo.vewPos = vg::Vector4(m_pCamera->getTransform()->getLocalPosition(), 1.0f);
}