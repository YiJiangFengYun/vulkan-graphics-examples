#include "texture_array/window.hpp"

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
    , m_tempPositions()
    , m_tempTexCoords()
    , m_tempIndices()
    , m_pModel()
    , m_pMesh()
    , m_pTexture()
    , m_pMaterial()
    , m_instanceCount(0u)
{
    _init();
}

Window::Window(std::shared_ptr<GLFWwindow> pWindow
    , std::shared_ptr<vk::SurfaceKHR> pSurface
)
    : sampleslib::Window<vg::SpaceType::SPACE_3>(pWindow
        , pSurface
        )
    , m_tempPositions()
    , m_tempTexCoords()
    , m_tempIndices()
    , m_pModel()
    , m_pMesh()
    , m_pTexture()
    , m_pMaterial()
    , m_instanceCount(0u)
{
    _init();
}

void Window::_init()
{
    ParentWindowType::_init();
    _loadModel();
    _createMesh();
    _createTexture();
    _createMaterial();
    _createModel();    
}

void Window::_initState()
{
    ParentWindowType::_initState();
    m_cameraZoom = -15.0f;
    /// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
    m_cameraRotation = vg::Vector3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
}

void Window::_loadModel()
{
    m_tempPositions = { vg::Vector3(2.5f, 2.5f, 0.0f)
        , vg::Vector3(-2.5f, 2.5f, 0.0f)
        , vg::Vector3(-2.5f, -2.5f, 0.0f)
        , vg::Vector3(2.5f, -2.5f, 0.0f)
    };
    m_tempTexCoords = { vg::Vector2(1.0f, 1.0f)
        , vg::Vector2(0.0f, 1.0f)
        , vg::Vector2(0.0f, 0.0f)
        , vg::Vector2(1.0f, 0.0f)
    };
    m_tempIndices = {
        0, 1, 2, 2, 3, 0
    };
}

void Window::_createMesh()
{
    m_pMesh = static_cast<std::shared_ptr<vg::DimSepMesh3>>(new vg::DimSepMesh3());
    m_pMesh->setVertexCount(static_cast<uint32_t>(m_tempPositions.size()));
    m_pMesh->addPositions(m_tempPositions);
    m_pMesh->addTextureCoordinates<vg::TextureCoordinateType::VECTOR_2, vg::TextureCoordinateIndex::TextureCoordinate_0>(m_tempTexCoords);
    m_pMesh->setIndices(m_tempIndices, vg::PrimitiveTopology::TRIANGLE_LIST, 0u);
    m_pMesh->apply(VG_TRUE);
    m_pMesh->setIsHasBounds(VG_FALSE);
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
        fileName = "textures/texturearray_bc3_unorm.ktx";
        format = vk::Format::eBc2UnormBlock;
    }
    else if (deviceFeatures.textureCompressionASTC_LDR)
    {
        fileName = "textures/texturearray_astc_8x8_unorm.ktx";
        format = vk::Format::eAstc8x8UnormBlock;
    }
    else if (deviceFeatures.textureCompressionETC2)
    {
        fileName = "textures/texturearray_etc2_unorm.ktx";
        format = vk::Format::eEtc2R8G8B8UnormBlock;
    }
    else
    {
        throw std::runtime_error("Device does not support any compressed texture format!");
    }

    gli::texture2d_array gliTex(gli::load(fileName));
    if (gliTex.empty()) {
        throw std::runtime_error("The texture do't exist! path: " + fileName);
    }

    auto pTex = new vg::Texture2DArray(format, VG_TRUE,
        gliTex[0].extent().x,
        gliTex[0].extent().y,
        gliTex.layers()
    );
    m_pTexture = std::shared_ptr<vg::Texture2DArray>(pTex);
    uint32_t mipLevels = static_cast<uint32_t>(gliTex.levels());
    uint32_t layerCount = static_cast<uint32_t>(gliTex.layers());
    uint32_t count = mipLevels * layerCount;
    vg::TextureDataInfo textureLayout;
    std::vector<vg::TextureDataInfo::Component> components(count);
    for (uint32_t layer = 0; layer < layerCount; ++layer) {
        for (uint32_t level = 0; level < mipLevels; ++level) {
            uint32_t index = layer * mipLevels + level;
            components[index].mipLevel = level;
            components[index].baseArrayLayer = layer;
            components[index].layerCount = 1u;
            components[index].size = gliTex[layer][level].size();
            components[index].hasImageExtent = VG_TRUE;
            components[index].width = gliTex[layer][level].extent().x;
            components[index].height = gliTex[layer][level].extent().y;
            components[index].depth = 1u;
        }
    }
    textureLayout.componentCount = components.size();
    textureLayout.pComponent = components.data();
    m_pTexture->applyData(textureLayout, gliTex.data(), gliTex.size());

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
        (float)(m_pTexture->getImage()->getInfo().mipLevels),
    };
    m_pTexture->createSampler("other_sampler", info);


    m_instanceCount = std::min(m_pTexture->getArrayLength(), MAX_INSTANCE_COUNT);
}

void Window::_createMaterial()
{
    auto & pApp = vg::pApp;
    //material
    auto & pMaterial = m_pMaterial;
    pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
    pMaterial->setRenderPriority(0u);
    pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);

    auto pShader = pMaterial->getMainShader();
    auto pPass = pMaterial->getMainPass();
    
    //shader
    pShader->load("shaders/texture_array/instancing.vert.spv", "shaders/texture_array/instancing.frag.spv");
    //pass
    vg::Pass::BuildInDataInfo::Component buildInDataCmps[2] = {
            {vg::Pass::BuildInDataType::MATRIX_PROJECTION},
            {vg::Pass::BuildInDataType::MATRIX_VIEW}
        };
        vg::Pass::BuildInDataInfo buildInDataInfo;
        buildInDataInfo.componentCount = 2u;
        buildInDataInfo.pComponent = buildInDataCmps;
    pPass->setBuildInDataInfo(buildInDataInfo);
    pPass->setCullMode(vk::CullModeFlagBits::eNone);
    pPass->setFrontFace(vk::FrontFace::eClockwise);
    vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
    depthStencilState.depthTestEnable = VG_TRUE;
    depthStencilState.depthWriteEnable = VG_TRUE;
    depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
    pPass->setDepthStencilInfo(depthStencilState);
    vg::PassTextureInfo::TextureInfo itemInfo = {
        m_pTexture.get(),
        nullptr,
        m_pTexture->getSampler("other_sampler"),
        vk::ImageLayout::eUndefined,
    };
    vg::PassTextureInfo mainTextureInfo = {
        vg::SamplerTextureType::TEX_2D_ARRAY,
        1u,
        &itemInfo,
        VG_PASS_OTHER_MIN_BINDING_PRIORITY,
        vg::ImageDescriptorType::COMBINED_IMAGE_SAMPLER,
        vk::ShaderStageFlagBits::eFragment,
    };
    pPass->addTexture("main_texture", mainTextureInfo);
    vg::PassDataInfo otherDataInfo = {
        VG_PASS_OTHER_DATA_MIN_LAYOUT_PRIORITY,
        vk::ShaderStageFlagBits::eVertex,
    };
    pPass->addData("other_info", otherDataInfo, m_otherInfo);
    pPass->setInstanceCount(m_instanceCount);
    pPass->apply();
    
    pMaterial->apply();

}

void Window::_createModel()
{
    m_pModel = std::shared_ptr<vg::VisualObject3>(new vg::VisualObject3());
    m_pModel->setMesh(m_pMesh.get());
    m_pModel->setMaterialCount(1u);
    m_pModel->setMaterial(m_pMaterial.get());
    // m_pModel->setIsVisibilityCheck(VG_FALSE);
    m_pScene->addVisualObject(m_pModel.get());
}

void Window::_onUpdate()
{
    ParentWindowType::_onUpdate();

    uint32_t instanceCount = m_instanceCount;
    OtherInfo &otherInfo = m_otherInfo;

    // Array indices and model matrices are fixed
    float offset = 1.5f;
    float center = (instanceCount * offset) / 2;
    for (uint32_t i = 0u; i < instanceCount; ++i)
    {
        // Instance model matrix
        otherInfo.instance[i].model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, i * offset - center, 0.0f));
        otherInfo.instance[i].model = glm::rotate(otherInfo.instance[i].model, glm::radians(60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // Instance texture array index
        otherInfo.instance[i].arrayIndex.x = static_cast<float>(i);
    }
    auto pPass = m_pMaterial->getMainPass();
    pPass->setData("other_info", m_otherInfo);
    pPass->apply();
}