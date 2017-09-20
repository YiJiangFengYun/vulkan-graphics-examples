#ifndef KGS_MESH_H
#define KGS_MESH_H

#include <glm/glm.hpp>
#include <foundation/foundation.hpp>
#include "graphics/global.hpp"
#include "graphics/mesh/mesh_option.hpp"
#include "graphics/mesh/mesh_data.hpp"

namespace kgs
{
	template <MeshType meshType>
	class Mesh
	{
	public:
		struct LayoutInfo
		{
			std::string name;
			MeshData::DataType dataType;
			uint32_t location;

			LayoutInfo(std::string name,
				MeshData::DataType dataType,
				uint32_t location) :
				name(name),
				dataType(dataType),
				location(location)
			{

			}

			Bool32 operator ==(const LayoutInfo& target) const
			{
				return name == target.name && dataType == target.dataType && location == target.location;
			}
		};

		struct SubMeshInfo
		{
			PrimitiveTopology topology;
			std::vector<uint32_t> indices;
		};

		const MeshData::DataType static BASE_DATA_TYPE = MeshTypeInfo<meshType>::BASE_TYPE;
		const MeshData::DataType static ARRAY_DATA_TYPE = MeshTypeInfo<meshType>::ARRAY_TYPE;
		typedef typename MeshData::DataTypeInfo<BASE_DATA_TYPE>::ValueType BaseValueType;
		typedef typename MeshData::DataTypeInfo<ARRAY_DATA_TYPE>::ValueType ArrayValueType;

		Mesh() :
			m_pContext(pContext);
			m_multipliedColor(COLOR_WHITE) //default multiplied color should be (1, 1, 1, 1)
		{

		}

		~Mesh()
		{

		}

		MeshType getMeshType() const
		{
			return m_meshType;
		}

		uint32_t getVertexCount() const
		{
			return m_vertexCount;
		}

		void setVertexCount(uint32_t value)
		{
			m_vertexCount = value;
		}

		//vertex
		ArrayValueType getVertices() const
		{
			return m_pData->getDataValue<ARRAY_DATA_TYPE>(KGS_VERTEX_POSTION_NAME);
		}

		void setVertices(ArrayValueType vertices)
		{
			m_pData->setDataValue<ARRAY_DATA_TYPE>(KGS_VERTEX_POSTION_NAME, vertices);
		}

		//color
		std::vector<Color32> getColors() const
		{
			return m_pData->getDataValue<MeshData::DataType::COLOR_32_ARRAY>(KGS_VERTEX_COLOR_NAME);
		}

		void setColors(std::vector<Color32> colors)
		{
			m_pData->setDataValue<MeshData::DataType::COLOR_32_ARRAY>(KGS_VERTEX_COLOR_NAME, colors);
		}

		//normal
		ArrayValueType getNormals() const
		{
			return m_pData->getDataValue<ARRAY_DATA_TYPE>(KGS_VERTEX_NORMAL_NAME);
		}

		void setNormals(ArrayValueType normals)
		{
			m_pData->setDataValue<ARRAY_DATA_TYPE>(KGS_VERTEX_NORMAL_NAME, normals);
		}

		//tangent
		ArrayValueType getTangents() const
		{
			return m_pData->getDataValue<ARRAY_DATA_TYPE>(KGS_VERTEX_TANGENT_NAME);
		}

		void setTangents(ArrayValueType tangents)
		{
			m_pData->setDataValue<ARRAY_DATA_TYPE>(KGS_VERTEX_TANGENT_NAME, tangents);
		}

		//uv
		template<UVType uvType, UVIndex uvIndex>
		typename MeshData::DataTypeInfo<UVTypeInfo<uvType>::ARRAY_TYPE>::ValueType getUVs()
		{
			return m_pData->getDataValue<UVTypeInfo<uvType>::ARRAY_TYPE>(UVIndexInfo<uvIndex>::VERTEX_NAME);
		}

		template<UVType uvType, UVIndex uvIndex>
		void setUVs(typename MeshData::DataTypeInfo<UVTypeInfo<uvType>::ARRAY_TYPE>::ValueType uvs, uint32_t uvIndex)
		{
			m_pData->setDataValue<UVTypeInfo<uvType>::ARRAY_TYPE>(UVIndexInfo<uvIndex>::VERTEX_NAME, uvs);
		}

		//index
		uint32_t getSubMeshCount() const
		{
			return m_subMeshCount;
		}

		void setSubMeshCount(uint32_t value)
		{
			m_subMeshCount = value;
			if (static_cast<uint32_t>(m_subMeshInfos.size()) < value)
			{
				m_subMeshInfos.resize(value);
			}
		}

		std::vector<uint32_t> getIndices(uint32_t subMeshIndex) const
		{
#ifdef DEBUG
			verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
			return m_subMeshInfos[subMeshIndex];
		}

		void setIndices(std::vector<uint32_t> indices, PrimitiveTopology topology, uint32_t subMeshIndex)
		{
#ifdef DEBUG
			verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
			SubMeshInfo& subMeshInfo = m_subMeshInfos[subMeshIndex];
			subMeshInfo.topology = topology;
			subMeshInfo.indices = indices;
		}

		uint32_t getIndexCount(uint32_t subMeshIndex) const
		{
#ifdef DEBUG
			verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
			return _getIndexCount(subMeshIndex);
		}

		uint32_t getIndexStart(uint32_t subMeshIndex) const
		{
#ifdef DEBUG
			verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
			uint32_t start = 0;
			for (uint32_t i = 0; i < subMeshIndex; ++i)
			{
				start += _getIndexCount(i);
			}
			return start;
		}

		PrimitiveTopology getTopology(uint32_t subMeshIndex) const
		{
#ifdef DEBUG
			verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
			return m_subMeshInfos[subMeshIndex].topology;
		}

		/*The bounding volume of the mesh*/
		fd::Bounds<typename BaseValueType> getBounds()
		{
			return m_bounds;
		}

		/**Vertex colors of the Mesh multiplied to verties*/
		Color getMultipliedColor() const
		{
			return m_multipliedColor;
		}

		/**Vertex colors of the Mesh multiplied to verties*/
		void setMultipliedColor(Color value)
		{
			m_multipliedColor = value;
		}

		/**Vertex colors of the Mesh added to verties*/
		Color getAddedColor() const
		{
			return m_addedColor;
		}

		/**Vertex colors of the Mesh added to verties*/
		void setAddedColor(Color value)
		{
			m_addedColor = value;
		}

		void apply(Bool32 makeUnreadable)
		{
			//create vertex buffer
			{
				//get size of every vertex
				uint32_t size = 0;
				for (const auto& layoutInfo : m_layouts)
				{
					size += MeshData::getDataBaseTypeSize(layoutInfo.dataType);
				}
				//get vertex buffer size.
				uint32_t vertexBufferSize = size * m_vertexCount;

				//create staging buffer.
				vk::BufferCreateInfo createInfo = {
					vk::BufferCreateFlags(),
					vertexBufferSize,
					vk::BufferUsageFlagBits::eTransferSrc,
					vk::SharingMode::eExclusive
				};

				auto pDevice = m_pContext->getPNativeDevice();
				auto pStagingBuffer = fd::createBuffer(pDevice, createInfo);

				vk::MemoryRequirements memReqs = pDevice->getBufferMemoryRequirements(*pBuffer);
				vk::MemoryAllocateInfo allocateInfo = {
					memReqs.size,
					kgs::_findMemoryType(m_pContext->getPPhysicalDevice(),
					memReqs.memoryTypeBits,
					vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)
				};

				auto pStagingBufferMemory = fd::allocateMemory(pDevice, allocateInfo);

				pDevice->bindBufferMemory(*pStagingBuffer, *pStagingBufferMemory, 0);

				void* data;
				pDevice->mapMemory(*pStagingBufferMemory, 0u, static_cast<vk::DeviceSize>(vertexBufferSize), vk::MemoryMapFlags(), &data);
				uint32_t offset = 0;
				for (const auto& layoutInfo : m_layouts)
				{
					m_pData->memCopyDataValue(layoutInfo.name, layoutInfo.dataType, data, offset, 0u, m_vertexCount);
					offset += MeshData::getDataBaseTypeSize(layoutInfo.dataType) * m_vertexCount;
				}
				pDevice->unmapMemory(*pStagingBufferMemory);
			}




			//create index buffer
		}

	private:
		std::shared_ptr<Context> m_pContext;
		MeshType m_meshType = meshType;
		uint32_t m_vertexCount;
		std::shared_ptr<MeshData> m_pData;
		std::vector<LayoutInfo> m_layouts;
		uint32_t m_busMeshCount;
		std::vector<SubMeshInfo> m_subMeshInfos;
		fd::Bounds<BaseValueType> m_bounds;
		Color m_multipliedColor;
		Color m_addedColor;

		vk::Buffer vertexBuffer;
		vk::DeviceMemory vertexBufferMemory;
		vk::Buffer indexBuffer;
		vk::DeviceMemory indexBufferMemory;

#ifdef DEBUG
		inline verifySubMeshIndex(uint32_t subMeshIndex)
		{
			if (subMeshIndex >= m_busMeshCount)
				throw std::range_error("The subMeshIndex out of range of the actual sub mesh count.");
		}
#endif // DEBUG

		inline uint32_t _getIndexCount(uint32_t subMeshIndex) const
		{
			SubMeshInfo& subMeshInfo = m_subMeshInfos[subMeshIndex];
			return static_cast<uint32_t>(subMeshInfo.indices.size());
		}
	};
}

#endif // !KGS_MESH_H
