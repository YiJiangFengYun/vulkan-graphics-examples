#include "sampleslib/scene_assimp.hpp"

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>     
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

namespace sampleslib
{
    AssimpScene::CreateInfo::CreateInfo(const char* fileName
                , uint32_t layoutComponentCount
                , const VertexLayoutComponent *pLayoutComponent
                , vg::Vector3 offset
                , vg::Vector3 scale
                , vg::Vector2 uvScale
                , vg::Bool32 isCreateObject
                )
    {
        this->fileName = fileName;
        this->layoutComponentCount = layoutComponentCount;
        this->pLayoutComponent = pLayoutComponent;
        this->offset = offset;
        this->scale = scale;
        this->uvScale = uvScale;
        this->isCreateObject = isCreateObject;
    }

    AssimpScene::AssimpScene()
        : m_pMeshes()
        , m_pObjects()
        , m_pSharedVertexData(new vg::VertexData())
		, m_pSharedIndexData(new vg::IndexData())
	{
		
	}

    AssimpScene::AssimpScene(const CreateInfo &createInfo)
        : m_pMeshes()
        , m_pObjects()
        , m_pSharedVertexData(new vg::VertexData())
		, m_pSharedIndexData(new vg::IndexData())
    {
    }

    void AssimpScene::init(const CreateInfo &createInfo)
    {
        _init(createInfo);
    }

    const std::vector<std::shared_ptr<vg::DimSharedContentMesh3>> AssimpScene::getMeshes() const
    {
        return m_pMeshes;
    }

    const std::vector<std::shared_ptr<vg::VisualObject3>> AssimpScene::getObjects() const
    {
        return m_pObjects;
    }

    void AssimpScene::_init(const CreateInfo &createInfo)
    {
        const auto &device = vg::pApp->getDevice();
        Assimp::Importer importer;
        const aiScene* pScene;

        const int flags = aiProcess_FlipWindingOrder | aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals;

        pScene = importer.ReadFile(createInfo.fileName, flags);

        if (!pScene)
        {
            std::string error = importer.GetErrorString();
            throw std::runtime_error(error);
        }
        else
        {
            uint32_t meshCount = pScene->mNumMeshes;
            //Constructing the shared vertex buffer data.
            auto &pSharedVertexData = m_pSharedVertexData;
            auto &pSharedIndexData = m_pSharedIndexData;

            uint32_t vertexSize = 0u;
            {
                //Constructing vertex input binding description and attribute description.
                uint32_t componentCount = createInfo.layoutComponentCount;
                const VertexLayoutComponent *pComponent = createInfo.pLayoutComponent;
                uint32_t binding = 0u;
                uint32_t stride = 0u;
                uint32_t offset = 0u;
                vk::Format format;
                std::vector<vk::VertexInputAttributeDescription> attributeDescs(componentCount);
                for (uint32_t i = 0; i < componentCount; ++i)
                {
                    switch (*(pComponent + i))
                    {
                    case VertexLayoutComponent::VERTEX_COMPONENT_UV:
                        stride += 2u * static_cast<uint32_t>(sizeof(float));
                        format = vk::Format::eR32G32Sfloat;
                        break;
                    case VertexLayoutComponent::VERTEX_COMPONENT_DUMMY_FLOAT:
                        stride += static_cast<uint32_t>(sizeof(float));
                        format = vk::Format::eR32Sfloat;
                        break;
                    case VertexLayoutComponent::VERTEX_COMPONENT_DUMMY_VEC4:
                        stride += 4u * static_cast<uint32_t>(sizeof(float));
                        format = vk::Format::eR32G32B32A32Sfloat;
                        break;
                    default:
                        // All components except the ones listed above are made up of 3 floats
			    		stride += 3u * static_cast<uint32_t>(sizeof(float));
                        format = vk::Format::eR32G32B32Sfloat;
                    }
    
                    attributeDescs[i].binding = binding;
                    attributeDescs[i].location = i;
                    attributeDescs[i].format = format;
                    attributeDescs[i].offset = offset;
                    
                    offset = stride;
                }
                vk::VertexInputBindingDescription bindingDesc[1] = {};
                bindingDesc[0].binding = binding;
                bindingDesc[0].stride = stride;
                bindingDesc[0].inputRate = vk::VertexInputRate::eVertex;
    
                //Constructing pipeline vertex input state create info.
                vk::PipelineVertexInputStateCreateInfo vertexInfo = {};
                vertexInfo.vertexBindingDescriptionCount = 1u;
                vertexInfo.pVertexBindingDescriptions = bindingDesc;
                vertexInfo.vertexAttributeDescriptionCount = attributeDescs.size();
                vertexInfo.pVertexAttributeDescriptions = attributeDescs.data();
        
                //Constructing pipeline input assembly state create info.
                vk::PipelineInputAssemblyStateCreateInfo iaInfo = {};
                iaInfo.topology = vk::PrimitiveTopology::eTriangleList;
    
                pSharedVertexData->updateDesData(vertexInfo);
                pSharedIndexData->updateDesData(vk::IndexType::eUint32, iaInfo);

                vertexSize = stride;
            }

            std::vector<fd::Bounds<vg::Vector3>> boundses(meshCount);

            {
                uint32_t vertexSubDataCount = meshCount;
                uint32_t indexSubDataCount = meshCount;
    
                vg::Vector3 scale = createInfo.scale;
                vg::Vector2 uvScale = createInfo.uvScale;
                vg::Vector3 offset = createInfo.offset;
    
                std::vector<uint32_t> vertexCounts(vertexSubDataCount);
                std::vector<uint32_t> vertexBufferSizes(vertexSubDataCount);
    
                std::vector<uint32_t> indexCounts(indexSubDataCount);
                std::vector<uint32_t> indexBufferSizes(indexSubDataCount);
                std::vector<uint32_t> indexVertexDataIndices(indexSubDataCount);
        
                std::vector<float> vertexBuffer;
                std::vector<uint32_t> indexBuffer;
    
                for (uint32_t i = 0; i < meshCount; ++i)
                {
                    const aiMesh *paiMesh = pScene->mMeshes[i];
    
                    aiColor3D tempColor(0.0f, 0.0f, 0.0f);
                    pScene->mMaterials[paiMesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, tempColor);
                    const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

                    vg::Vector3 minOfBounds(std::numeric_limits<typename vg::Vector3::value_type>::max());
                    vg::Vector3 maxOfBounds(std::numeric_limits<typename vg::Vector3::value_type>::lowest());
    
                    uint32_t vertexCount = paiMesh->mNumVertices;
                    vertexCounts[i] = vertexCount;

                    uint32_t vertexBufferSize = vertexSize * vertexCount;
					uint32_t indexCount = 0u;
                    uint32_t indexBufferSize = 0u;          
                    for (uint32_t j = 0; j < vertexCount; ++j)
                    {
                        const aiVector3D *pPos = &(paiMesh->mVertices[j]);
                        const aiVector3D *pNormal = &(paiMesh->mNormals[j]);
                        const aiVector3D *pTexCoord = (paiMesh->HasTextureCoords(0u)) ? &(paiMesh->mTextureCoords[0][j]) : &zero3D;
                        const aiVector3D *pTangent = (paiMesh->HasTangentsAndBitangents()) ? &(paiMesh->mTangents[j]) : &zero3D;
                        const aiVector3D *pBitangent = (paiMesh->HasTangentsAndBitangents()) ? &(paiMesh->mBitangents[j]) : &zero3D;
    
                        uint32_t componentCount = createInfo.layoutComponentCount;
                        const VertexLayoutComponent *pComponent = createInfo.pLayoutComponent;
                        uint32_t componentIndex;
                        for (componentIndex = 0u; componentIndex < componentCount; ++componentIndex)
                        {
							switch (*(pComponent + componentIndex))
							{
							case VertexLayoutComponent::VERTEX_COMPONENT_POSITION:
							{
								float x = pPos->x * scale.x + offset.x;
								float y = pPos->y * scale.y + offset.y;
								float z = pPos->z * scale.z + offset.z;
								vertexBuffer.push_back(x);
								vertexBuffer.push_back(y);
								vertexBuffer.push_back(z);

								//bounds...
								if (minOfBounds.x > x)minOfBounds.x = x;
								if (minOfBounds.y > y)minOfBounds.y = y;
								if (minOfBounds.z > z)minOfBounds.z = z;

								if (maxOfBounds.x < x)maxOfBounds.x = x;
								if (maxOfBounds.y < y)maxOfBounds.y = y;
								if (maxOfBounds.z < z)maxOfBounds.z = z;

								break;
							}
							case VertexLayoutComponent::VERTEX_COMPONENT_NORMAL:
							{
								vertexBuffer.push_back(pNormal->x);
								vertexBuffer.push_back(pNormal->y);
								vertexBuffer.push_back(pNormal->z);
								break;
							}
							case VertexLayoutComponent::VERTEX_COMPONENT_UV:
							{
								vertexBuffer.push_back(pTexCoord->x * uvScale.s);
								vertexBuffer.push_back(pTexCoord->y * uvScale.t);
								break;
							}
							case VertexLayoutComponent::VERTEX_COMPONENT_COLOR:
							{
								vertexBuffer.push_back(tempColor.r);
								vertexBuffer.push_back(tempColor.g);
								vertexBuffer.push_back(tempColor.b);
								break;
							}
							case VertexLayoutComponent::VERTEX_COMPONENT_TANGENT:
							{
								vertexBuffer.push_back(pTangent->x);
								vertexBuffer.push_back(pTangent->y);
								vertexBuffer.push_back(pTangent->z);
								break;
							}
							case VertexLayoutComponent::VERTEX_COMPONENT_BITANGENT:
							{
								vertexBuffer.push_back(pBitangent->x);
								vertexBuffer.push_back(pBitangent->y);
								vertexBuffer.push_back(pBitangent->z);
								break;
							}
							// Dummy components for padding
							case VertexLayoutComponent::VERTEX_COMPONENT_DUMMY_FLOAT:
							{
								vertexBuffer.push_back(0.0f);
								break;
							}
							case VertexLayoutComponent::VERTEX_COMPONENT_DUMMY_VEC4:
							{
								vertexBuffer.push_back(0.0f);
								vertexBuffer.push_back(0.0f);
								vertexBuffer.push_back(0.0f);
								vertexBuffer.push_back(0.0f);
								break;
							}
                            }
                        }
                    }

                    vertexBufferSizes[i] = vertexBufferSize;
                    //bounds...
                    boundses[i].setMinMax(minOfBounds, maxOfBounds);

                    uint32_t faceCount = paiMesh->mNumFaces;
                    for (uint32_t j = 0; j < faceCount; ++j)
                    {
                        const aiFace &face = paiMesh->mFaces[j];
                        if (face.mNumIndices != 3)
                            continue;
                        indexBuffer.push_back(face.mIndices[0]);
                        indexBuffer.push_back(face.mIndices[1]);
                        indexBuffer.push_back(face.mIndices[2]);
                        indexCount += 3u;
                    }

                    indexBufferSize = indexCount * 32u;

                    indexCounts[i] = indexCount;
                    indexBufferSizes[i] = indexBufferSize;
                    indexVertexDataIndices[i] = i;
                }

                pSharedVertexData->updateBuffer(vertexBuffer.data(), 
                    static_cast<uint32_t>(vertexBuffer.size() * sizeof(float)),
                    VG_FALSE);

                pSharedIndexData->updateBuffer(indexBuffer.data(),
                    static_cast<uint32_t>(indexBuffer.size() * sizeof(uint32_t)),
                    VG_FALSE);
        
                pSharedVertexData->updateSubDataCount(vertexSubDataCount);
		        if (vertexSubDataCount)
		        {
		        	const auto &firstSubVertexData = pSharedVertexData->getSubVertexDatas()[0];
		        	pSharedVertexData->updateDesData(firstSubVertexData.vertexInputStateInfo);
		        	pSharedVertexData->updateVertexCount(vertexCounts, vertexSubDataCount);
		        	pSharedVertexData->updateBufferSize(vertexBufferSizes, vertexSubDataCount);
		        }
        
                pSharedIndexData->updateSubDataCount(indexSubDataCount);
		        if (indexSubDataCount)
		        {
		        	const auto &firstSubIndexData = pSharedIndexData->getSubIndexDatas()[0];
		        	pSharedIndexData->updateDesData(firstSubIndexData.indexType, firstSubIndexData.inputAssemblyStateInfo);
		        	pSharedIndexData->updateIndexCount(indexCounts, indexSubDataCount);
		        	pSharedIndexData->updateBufferSize(indexBufferSizes, indexSubDataCount);
		        	pSharedIndexData->updateVertexDataIndex(indexVertexDataIndices, indexSubDataCount);
		        }
            }

            {
                //Filling the meshes vertex buffer data.
                auto &pMeshes = m_pMeshes;
                pMeshes.resize(meshCount);
    
                for (uint32_t i = 0; i < meshCount; ++i)
                {
                    auto &pMesh = pMeshes[i];
					pMesh = std::shared_ptr<vg::DimSharedContentMesh3>(new vg::DimSharedContentMesh3());
                    pMesh->init(pSharedVertexData, pSharedIndexData, i, 1u);
                    pMesh->setIsHasBounds(VG_TRUE);
                    pMesh->setBounds(boundses[i]);
                }
            }

            {
                //Filling the visual object.
                if (createInfo.isCreateObject)
                {
                    auto &pObjects = m_pObjects;
                    auto &pMeshes = m_pMeshes;
                    pObjects.resize(meshCount);

                    for (uint32_t i = 0; i < meshCount; ++i)
                    {
						pObjects[i] = std::shared_ptr<vg::VisualObject3>(new vg::VisualObject3());
                        pObjects[i]->setMesh(pMeshes[i]);
                    }
                }
                else
                {
                    m_pObjects.resize(0);
                }
            }
    
        }

    }
}