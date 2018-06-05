#include "chalet/window.hpp"

#define MODEL_PATH "models/chalet.obj"
#define TEXTURE_PATH "textures/chalet.jpg"

namespace chalet
{
    bool Window::Vertex::operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }

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
        _loadModel();
        _createMesh();
        _createTexture();
        _createMaterial();
        _createModel();
        _createMeshOfBounds();
        _createMaterialOfBounds();
        _createModelOfBounds();
    }

    void Window::_loadModel()
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, MODEL_PATH))
        {
            throw std::runtime_error(err);
        }

        std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

        for (const auto& shape : shapes)
        {
            for (const auto& index : shape.mesh.indices)
            {
                Vertex vertex = {};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = { 1.0f, 1.0f, 1.0f };

                if (uniqueVertices.count(vertex) == 0)
                {
                    uniqueVertices[vertex] = static_cast<uint32_t>(m_tempPositions.size());
                    m_tempPositions.push_back(vertex.pos);
                    m_tempTexCoords.push_back(vertex.texCoord);
                    m_tempColors.push_back(vg::Color(vertex.color, 1.0f));
                }

                m_tempIndices.push_back(uniqueVertices[vertex]);
            }
        }
    }

    void Window::_createMesh()
    {
        m_pMesh = static_cast<std::shared_ptr<vg::DimSepMesh3>>(new vg::DimSepMesh3());
        m_pMesh->setVertexCount(static_cast<uint32_t>(m_tempPositions.size()));
        m_pMesh->addPositions(m_tempPositions);
        m_pMesh->addTextureCoordinates<vg::TextureCoordinateType::VECTOR_2, vg::TextureCoordinateIndex::TextureCoordinate_0>(m_tempTexCoords);
        m_pMesh->setIndices(m_tempIndices, vg::PrimitiveTopology::TRIANGLE_LIST, 0u);
        m_pMesh->apply(VG_TRUE);
    }

    void Window::_createTexture()
    {
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(TEXTURE_PATH, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        uint32_t imageSize = texWidth * texHeight * 4;

        if (!pixels) {
            throw std::runtime_error("Failed to load texture image!");
        }
        
        m_pTexture = std::shared_ptr<vg::Texture2D>(new vg::Texture2D(vk::Format::eR8G8B8A8Unorm, VG_FALSE, texWidth, texHeight));
        vg::TextureDataInfo layoutInfo;
        vg::TextureDataInfo::Component component;
        component.mipLevel = 0u;
        component.layerCount = 1u;
        component.baseArrayLayer = 0u;
        component.size = imageSize;
        layoutInfo.componentCount = 1u;
        layoutInfo.pComponent = &component;
        m_pTexture->applyData(layoutInfo, reinterpret_cast<void *>(pixels), imageSize);
    }

    void Window::_createMaterial()
    {
        m_pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
        m_pMaterial->setRenderPriority(0u);
        m_pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);

        auto pShader = m_pMaterial->getMainShader();
        auto pPass = m_pMaterial->getMainPass();
        pShader->load("shaders/chalet.vert.spv",
            "shaders/chalet.frag.spv");
        pPass->setCullMode(vk::CullModeFlagBits::eBack);
        pPass->setFrontFace(vk::FrontFace::eClockwise);
        vk::PipelineDepthStencilStateCreateInfo depthStencilStateInfo;
        depthStencilStateInfo.depthTestEnable = VG_TRUE;
        depthStencilStateInfo.depthWriteEnable = VG_TRUE;
        depthStencilStateInfo.depthCompareOp = vk::CompareOp::eLess;
        pPass->setDepthStencilInfo(depthStencilStateInfo);
        vg::PassTextureInfo::TextureInfo itemInfo = {
            m_pTexture.get(),
            nullptr,
            nullptr,
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
        
        m_pMaterial->apply();
    }

    void Window::_createModel()
    {
        m_pModel = std::shared_ptr<vg::VisualObject3>(new vg::VisualObject3());
        m_pModel->setMesh(m_pMesh.get());
        auto pMaterial = m_pMaterial.get();
        m_pModel->setMaterialCount(1u);
        m_pModel->setMaterial(pMaterial);
        m_pScene->addVisualObject(m_pModel.get());
    }

    void Window::_createMeshOfBounds()
    {
        const auto &bounds = m_pMesh->getBounds();
        auto min = bounds.getMin();
        auto max = bounds.getMax();
        uint32_t len = static_cast<uint32_t>(vg::Vector3::length());
        uint32_t pointCount = static_cast<uint32_t>(std::pow(2u, len));
        std::vector<vg::Vector3> points(pointCount);
        std::vector<vg::Color32> colors(pointCount, vg::Color32(255, 255, 255, 255));
        uint32_t pointIndex = 0u;
        std::vector<vg::Bool32> places(len);
        for (uint32_t i = 0; i <= len; ++i)
        {
            std::fill(places.begin(), places.end(), VG_FALSE);
            std::fill(places.begin(), places.begin() + i, VG_TRUE);
            do
            {
                uint32_t j;
                for (j = 0; j < len; ++j)
                {
                    if (places[j] == VG_TRUE)
                    {
                        points[pointIndex][j] = min[j];
                    }
                    else
                    {
                        points[pointIndex][j] = max[j];
                    }
                }
                ++pointIndex;
            } while (std::prev_permutation(places.begin(), places.end()));
        }

        //Calculate the indices of the mesh of the bounds.
        uint32_t needSameDimCount = len - 1u;
        std::vector<uint32_t> indices(2u * len * static_cast<uint32_t>(std::pow(2u, len - 1u)));
        uint32_t indexOfIndices = 0u;
        for (uint32_t i = 0; i < pointCount; ++i)
        {
            for (uint32_t j = i + 1; j < pointCount; ++j)
            {
                uint32_t sameDimCount = 0u;
                for (uint32_t d = 0; d < len; ++d)
                {
                    if (points[i][d] == points[j][d])
                    {
                        ++sameDimCount;
                    }
                }

                if (sameDimCount == needSameDimCount)
                {
                    indices[indexOfIndices] = i;
                    ++indexOfIndices;
                    indices[indexOfIndices] = j;
                    ++indexOfIndices;
                }
            }
        }

        m_pMeshOfBounds = static_cast<std::shared_ptr<vg::DimSepMesh3>>(new vg::DimSepMesh3());
        m_pMeshOfBounds->setVertexCount(pointCount);
        m_pMeshOfBounds->addPositions(points);
        m_pMeshOfBounds->addColor32s(colors);
        m_pMeshOfBounds->setIndices(indices, vg::PrimitiveTopology::LINE_LIST, 0u);
        m_pMeshOfBounds->apply(VG_TRUE);
    }
        
    void Window::_createMaterialOfBounds()
    {
        m_pMaterialOfBounds = std::shared_ptr<vg::Material>(new vg::Material());
        m_pMaterialOfBounds->setRenderPriority(0u);
        m_pMaterialOfBounds->setRenderQueueType(vg::MaterialShowType::OPAQUE);
        auto pShader = m_pMaterialOfBounds->getMainShader();
        auto pPass = m_pMaterialOfBounds->getMainPass();
        pShader->load("shaders/only_color.vert.spv", 
            "shaders/only_color.frag.spv");
        pPass->setCullMode(vk::CullModeFlagBits::eNone);
        vg::Pass::BuildInDataInfo::Component buildInDataCmps[2] = {
            {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
            {vg::Pass::BuildInDataType::MAIN_CLOLOR}
        };
        vg::Pass::BuildInDataInfo buildInDataInfo;
        buildInDataInfo.componentCount = 2u;
        buildInDataInfo.pComponent = buildInDataCmps;
        pPass->setBuildInDataInfo(buildInDataInfo);
        vk::PipelineDepthStencilStateCreateInfo depthStencilStateInfo;
        depthStencilStateInfo.depthTestEnable = VG_FALSE;
        depthStencilStateInfo.depthWriteEnable = VG_FALSE;
        depthStencilStateInfo.depthCompareOp = vk::CompareOp::eNever;
        pPass->setDepthStencilInfo(depthStencilStateInfo);
        
        m_pMaterialOfBounds->apply();
    }

    void Window::_createModelOfBounds()
    {
        m_pModelOfBounds = std::shared_ptr<vg::VisualObject3>(new vg::VisualObject3());
        m_pModelOfBounds->setMesh(m_pMeshOfBounds.get());
        auto pMaterial = m_pMaterialOfBounds.get();
        m_pModelOfBounds->setMaterialCount(1u);
        m_pModelOfBounds->setMaterial(pMaterial);
        m_pScene->addVisualObject(m_pModelOfBounds.get());
    }

    void Window::_onUpdate()
    {
        ParentWindowType::_onUpdate();
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = currentTime - startTime;
        float time = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() / 1000.0f;

        auto pTransform = m_pModel->getTransform();
        auto pTransformOfBounds = m_pModelOfBounds->getTransform();
        pTransform->setLocalRotation(glm::angleAxis(glm::radians(90.0f) * time, vg::Vector3(0.0f, 0.0f, 1.0f)));
        pTransformOfBounds->setLocalRotation(glm::angleAxis(glm::radians(90.0f) * time, vg::Vector3(0.0f, 0.0f, 1.0f)));

        pTransform = m_pCamera->getTransform();
        //pTransform->setLocalPosition(kgs::Vector3(2.0f, 2.0f, 2.0f));
        pTransform->lookAt2(vg::Vector3(2.0f, 2.0f, 2.0f), vg::Vector3(0.0f, 0.0f, 0.0f), vg::Vector3(0.0f, 0.0f, 1.0f));
    }
}