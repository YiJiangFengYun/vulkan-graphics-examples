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
                , vg::Bool32 isRightHand
                , vg::Bool32 separateMesh
                )
                : fileName(fileName)
                , layoutComponentCount(layoutComponentCount)
                , pLayoutComponent(pLayoutComponent)
                , offset(offset)
                , scale(scale)
                , uvScale(uvScale)
                , isCreateObject(isCreateObject)
                , isRightHand(isRightHand)
                , separateMesh(separateMesh)
    {
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
                        int32_t yDelta = createInfo.isRightHand ? 1 : -1; //If it is right hand, y will be inversed.
                        for (componentIndex = 0u; componentIndex < componentCount; ++componentIndex)
                        {
							switch (*(pComponent + componentIndex))
							{
							case VertexLayoutComponent::VERTEX_COMPONENT_POSITION:
							{
                                float x = pPos->x;
                                float y;
                                float z = pPos->z;
                                if (! createInfo.isRightHand) {
                                    y = - pPos->y;
                                    vg::Vector4 temp(x, y, z, 1.0f);
                                    vg::Quaternion tempQ(vg::Vector3(glm::radians(180.0f), glm::radians(0.0f), glm::radians(0.0f)));
                                    temp = tempQ * temp;
                                    x = temp.x;
                                    y = temp.y;
                                    z = temp.z;
                                }
                                else 
                                {
								    y = pPos->y;
                                }

								x = x * scale.x + offset.x;
								y = y * scale.y + offset.y;
								z = z * scale.z + offset.z;
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
								float x = pNormal->x;
								float y;
								float z = pNormal->z;
								if (! createInfo.isRightHand) {
									y = - pNormal->y;
									vg::Vector4 temp(x, y, z, 1.0f);
									vg::Quaternion tempQ(vg::Vector3(glm::radians(180.0f), glm::radians(0.0f), glm::radians(0.0f)));
									temp = tempQ * temp;
									x = temp.x;
									y = temp.y;
									z = temp.z;
								}
								else {
									y = pNormal->y;
								}
								vertexBuffer.push_back(x);
								vertexBuffer.push_back(y);
								vertexBuffer.push_back(z);
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
								vertexBuffer.push_back(yDelta * pTangent->y);
								vertexBuffer.push_back(pTangent->z);
								break;
							}
							case VertexLayoutComponent::VERTEX_COMPONENT_BITANGENT:
							{
								vertexBuffer.push_back(pBitangent->x);
								vertexBuffer.push_back(yDelta * pBitangent->y);
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

                    indexBufferSize = indexCount * 4u;

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
                if (createInfo.separateMesh)
                {
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
                else
                {
                    auto &pMeshes = m_pMeshes;
                    pMeshes.resize(1u);

                    vg::Vector3 minOfBounds(std::numeric_limits<typename vg::Vector3::value_type>::max());
                    vg::Vector3 maxOfBounds(std::numeric_limits<typename vg::Vector3::value_type>::lowest());
                    for (uint32_t i = 0; i < meshCount; ++i)
                    {
                        //bounds...
                        float x = boundses[i].getMin().x;
                        float y = boundses[i].getMin().y;
                        float z = boundses[i].getMin().z;
						if (minOfBounds.x > x)minOfBounds.x = x;
						if (minOfBounds.y > y)minOfBounds.y = y;
						if (minOfBounds.z > z)minOfBounds.z = z;

                        x = boundses[i].getMax().x;
                        y = boundses[i].getMax().y;
                        z = boundses[i].getMax().z;
						if (maxOfBounds.x < x)maxOfBounds.x = x;
						if (maxOfBounds.y < y)maxOfBounds.y = y;
						if (maxOfBounds.z < z)maxOfBounds.z = z;
                    }

                    fd::Bounds<vg::Vector3> bounds(minOfBounds, maxOfBounds);

                    auto &pMesh = pMeshes[0];
                    pMesh = std::shared_ptr<vg::DimSharedContentMesh3>(new vg::DimSharedContentMesh3());
                    pMesh->init(pSharedVertexData, pSharedIndexData, 0, meshCount);
                    pMesh->setIsHasBounds(VG_TRUE);
                    pMesh->setBounds(bounds);
                }
                
            }

            {
                //Filling the visual object.
                if (createInfo.isCreateObject)
                {
                    auto &pMeshes = m_pMeshes;                    
                    uint32_t count = static_cast<uint32_t>(pMeshes.size());
                    auto &pObjects = m_pObjects;
                    pObjects.resize(count);

                    for (uint32_t i = 0; i < count; ++i)
                    {
						pObjects[i] = std::shared_ptr<vg::VisualObject3>(new vg::VisualObject3());
						pObjects[i]->setMesh(pMeshes[i].get());
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