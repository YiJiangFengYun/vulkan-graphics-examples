#include "texture/window.hpp"

#include <iostream>
#include <gli/gli.hpp>

Window::OtherInfo::OtherInfo()
    : viewPos(vg::Vector4())
    , lodBias(0.0f)
{

}

Window::OtherInfo::OtherInfo(vg::Vector4 viewPos, float lodBias)
    : viewPos(viewPos)
    , lodBias(lodBias)
{

}

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
    , m_tempNormals()
    , m_tempIndices()
    , m_pModel()
    , m_pMesh()
    , m_pTexture()
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
    , m_tempPositions()
    , m_tempTexCoords()
    , m_tempNormals()
    , m_tempIndices()
    , m_pModel()
    , m_pMesh()
    , m_pTexture()
    , m_pMaterial()
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
    m_cameraZoom = -2.5f;
    /// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
    m_cameraRotation = vg::Vector3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));

}

void Window::_loadModel()
{
    m_tempPositions = { vg::Vector3(1.0f, 1.0f, 1.0f)
        , vg::Vector3(-1.0f, 1.0f, 1.0f)
        , vg::Vector3(-1.0f, -1.0f, 1.0f)
        , vg::Vector3(1.0f, -1.0f, 1.0f)
    };
    m_tempTexCoords = { vg::Vector2(1.0f, 1.0f)
        , vg::Vector2(0.0f, 1.0f)
        , vg::Vector2(0.0f, 0.0f)
        , vg::Vector2(1.0f, 0.0f)
    };
    m_tempNormals = {vg::Vector3(0.0f, 0.0f, -1.0f)
        , vg::Vector3(0.0f, 0.0f, -1.0f)
        , vg::Vector3(0.0f, 0.0f, -1.0f)
        , vg::Vector3(0.0f, 0.0f, -1.0f)
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
    m_pMesh->addNormals(m_tempNormals);
    m_pMesh->addTextureCoordinates<vg::TextureCoordinateType::VECTOR_2, vg::TextureCoordinateIndex::TextureCoordinate_0>(m_tempTexCoords);
    m_pMesh->setIndices(m_tempIndices, vg::PrimitiveTopology::TRIANGLE_LIST, 0u);
    m_pMesh->apply(VG_TRUE);
}

void Window::_createTexture()
{
    //load texture
    std::string fileName = "textures/metalplate01_rgba.ktx";
    auto format = vk::Format::eR8G8B8A8Unorm;
    gli::texture2d gliTex2D(gli::load(fileName));
    if (gliTex2D.empty()) {
        throw std::runtime_error("The texture do't exist! path: " + fileName);
    }

    auto pTex = new vg::Texture2D(format, VG_TRUE, 
        gliTex2D[0].extent().x, 
        gliTex2D[0].extent().y
    );
    m_pTexture = std::shared_ptr<vg::Texture2D>(pTex);
    uint32_t mipLevels = static_cast<uint32_t>(gliTex2D.levels());
    vg::TextureDataInfo textureLayout;
    std::vector<vg::TextureDataInfo::Component> components(mipLevels);
    for (uint32_t i = 0; i < mipLevels; ++i)
    {
        components[i].mipLevel = i;
        components[i].baseArrayLayer = 0u;
        components[i].layerCount = 1u;
        components[i].size = gliTex2D[i].size();
        components[i].hasImageExtent = VG_TRUE;
        components[i].width = gliTex2D[i].extent().x;
        components[i].height = gliTex2D[i].extent().y;
        components[i].depth = 1u;
    }
    textureLayout.componentCount = components.size();
    textureLayout.pComponent = components.data();
    m_pTexture->applyData(textureLayout, gliTex2D.data(), gliTex2D.size());
    
    auto &pApp = vg::pApp;
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
    pShader->load("shaders/texture/texture.vert.spv", "shaders/texture/texture.frag.spv");
    //pass
    pPass->setCullMode(vk::CullModeFlagBits::eFront);
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
    pPass->apply();
    
    pMaterial->apply();

}

void Window::_createModel()
{
    m_pModel = std::shared_ptr<vg::VisualObject3>(new vg::VisualObject3());
    m_pModel->setMesh(m_pMesh.get());
    m_pModel->setMaterialCount(1u);
    m_pModel->setMaterial(m_pMaterial.get());
    m_pScene->addVisualObject(m_pModel.get());
}

void Window::_onUpdate()
{
    auto pPass = m_pMaterial->getMainPass();

    ParentWindowType::_onUpdate();
    m_otherInfo.viewPos = vg::Vector4(m_pCamera->getTransform()->getLocalPosition(), 1.0f);

#ifdef USE_IMGUI_BIND
    auto pos = m_lastWinPos;
    auto size = m_lastWinSize;
    ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y + size.y + 10));
    ImGui::SetNextWindowSize(ImVec2(0, 0));
    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    if (ImGui::SliderFloat("LOD bias", &m_otherInfo.lodBias, 0.0f, (float)m_pTexture->getImage()->getInfo().mipLevels)) {
        pPass->setData("other_info", m_otherInfo);
        pPass->apply();
    }
    ImGui::End();
#endif //USE_IMGUI_BIND
}