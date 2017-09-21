#ifndef KGS_MESH_H
#define KGS_MESH_H

#include <glm/glm.hpp>
#include <foundation/foundation.hpp>
#include "graphics/global.hpp"
#include "graphics/util/find_memory.hpp"
#include "graphics/util/single_time_command.hpp"
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
			//caculate bounds
			_updateBounds();

			//create vertex buffer
			_createVertexBuffer();

			//create index buffer
			_createIndexBuffer();
		}

	private:
		std::shared_ptr<Context> m_pContext;
		MeshType m_meshType = meshType;
		uint32_t m_vertexCount;
		std::shared_ptr<MeshData> m_pData;
		std::vector<LayoutInfo> m_layouts;
		uint32_t m_subMeshCount;
		std::vector<SubMeshInfo> m_subMeshInfos;
		std::vector<SubMeshInfo> m_usingSubMeshInfos; //save sub mesh info to render.
		fd::Bounds<BaseValueType> m_bounds;
		Color m_multipliedColor;
		Color m_addedColor;

		std::shared_ptr<vk::Buffer> m_pVertexBuffer;
		std::shared_ptr<vk::DeviceMemory> m_pVertexBufferMemory;
		std::shared_ptr<vk::Buffer> m_pIndexBuffer;
		std::shared_ptr<vk::DeviceMemory> m_pIndexBufferMemory;


		//tool methods
#ifdef DEBUG
		inline void verifySubMeshIndex(uint32_t subMeshIndex)
		{
			if (subMeshIndex >= m_subMeshCount)
				throw std::range_error("The subMeshIndex out of range of the actual sub mesh count.");
		}
#endif // DEBUG

		inline uint32_t _getIndexCount(uint32_t subMeshIndex) const
		{
			SubMeshInfo& subMeshInfo = m_subMeshInfos[subMeshIndex];
			return static_cast<uint32_t>(subMeshInfo.indices.size());
		}

		void _copyBuffer(std::shared_ptr<vk::Buffer>& pSrcBuffer, std::shared_ptr<vk::Buffer>& pDstBuffer, vk::DeviceSize size)
		{
			auto pCommandBuffer = beginSingleTimeCommands();

			vk::BufferCopy copyRegin = {};
			copyRegin.srcOffset = 0;
			copyRegin.dstOffset = 0;
			copyRegin.size = size;
			pCommandBuffer->copyBuffer(*pSrcBuffer, *pDstBuffer, copyRegin);

			endSingleTimeCommands(pCommandBuffer);
		}

		inline void _createVertexBuffer()
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

			auto pPhysicalDevice = m_pContext->getPPhysicalDevice();
			auto pDevice = m_pContext->getPNativeDevice();
			auto pStagingBuffer = fd::createBuffer(pDevice, createInfo);

			vk::MemoryRequirements memReqs = pDevice->getBufferMemoryRequirements(*pStagingBuffer);
			vk::MemoryAllocateInfo allocateInfo = {
				memReqs.size,
				kgs::findMemoryType(pPhysicalDevice, memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)
			};

			auto pStagingBufferMemory = fd::allocateMemory(pDevice, allocateInfo);

			pDevice->bindBufferMemory(*pStagingBuffer, *pStagingBufferMemory, 0u);

			void* data;
			pDevice->mapMemory(*pStagingBufferMemory, 0u, static_cast<vk::DeviceSize>(vertexBufferSize), vk::MemoryMapFlags(), &data);
			uint32_t offset = 0;
			for (const auto& layoutInfo : m_layouts)
			{
				m_pData->memCopyDataValue(layoutInfo.name, layoutInfo.dataType, data, offset, 0u, m_vertexCount);
				offset += MeshData::getDataBaseTypeSize(layoutInfo.dataType) * m_vertexCount;
			}
			pDevice->unmapMemory(*pStagingBufferMemory);

			//create vertex buffer
			createInfo.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer;
			m_pVertexBuffer = fd::createBuffer(pDevice, createInfo);
			memReqs = pDevice->getBufferMemoryRequirements(*m_pVertexBuffer);
			allocateInfo.allocationSize = memReqs.size;
			allocateInfo.memoryTypeIndex = kgs::findMemoryType(pPhysicalDevice, memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
			m_pVertexBufferMemory = fd::allocateMemory(pDevice, allocateInfo);
			pDevice->bindBufferMemory(*m_pVertexBuffer, *m_pVertexBufferMemory, 0u);

			//copy buffer from staging buffer to vertex buffer.
			_copyBuffer(pStagingBuffer, m_pVertexBuffer, vertexBufferSize);
		}

		inline void _createIndexBuffer()
		{
			m_usingSubMeshInfos = m_subMeshInfos;

			//get index buffer size
			uint32_t indexBufferSize = 0;
			for (const auto& item : m_usingSubMeshInfos)
			{
				std::vector<uint32_t>& indices = item.indices;
				indexBufferSize += indices.size() * sizeof(uint32_t);
			}

			//create staging buffer.
			vk::BufferCreateInfo createInfo = {
				vk::BufferCreateFlags(),
				indexBufferSize,
				vk::BufferUsageFlagBits::eTransferSrc,
				vk::SharingMode::eExclusive
			};

			auto pPhysicalDevice = m_pContext->getPPhysicalDevice();
			auto pDevice = m_pContext->getPNativeDevice();
			auto pStagingBuffer = fd::createBuffer(pDevice, createInfo);

			vk::MemoryRequirements memReqs = pDevice->getBufferMemoryRequirements(*pStagingBuffer);
			vk::MemoryAllocateInfo allocateInfo = {
				memReqs.size,
				kgs::findMemoryType(pPhysicalDevice, memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)
			};

			auto pStagingBufferMemory = fd::allocateMemory(pDevice, allocateInfo);

			pDevice->bindBufferMemory(*pStagingBuffer, *pStagingBufferMemory, 0u);

			void* data;
			pDevice->mapMemory(*pStagingBufferMemory, 0u, static_cast<vk::DeviceSize>(indexBufferSize), vk::MemoryMapFlags(), &data);
			uint32_t offset = 0;
			for (const auto& subMeshInfo : m_usingSubMeshInfos)
			{
				std::vector<uint32_t>& indices = subMeshInfo.indices;
				size_t size = indices.size() * sizeof(uint32_t);
				memcpy(data + offset, indices.data(), size);
				offset += size;
			}
			pDevice->unmapMemory(*pStagingBufferMemory);

			//create index buffer
			createInfo.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer;
			m_pIndexBuffer = fd::createBuffer(pDevice, createInfo);
			memReqs = pDevice->getBufferMemoryRequirements(*m_pIndexBuffer);
			allocateInfo.allocationSize = memReqs.size;
			allocateInfo.memoryTypeIndex = kgs::findMemoryType(pPhysicalDevice, memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
			m_pIndexBufferMemory = fd::allocateMemory(pDevice, allocateInfo);
			pDevice->bindBufferMemory(*m_pIndexBuffer, *m_pIndexBufferMemory, 0u);

			//copy buffer from staging buffer to index buffer.
			_copyBuffer(pStagingBuffer, m_pIndexBuffer, indexBufferSize);
		}

		inline void _updateBounds()
		{
			if (m_vertexCount == 0u)
			{
				m_bounds = {};
				return;
			}

			auto vertices = m_pData->getDataValue<ARRAY_DATA_TYPE>(KGS_VERTEX_POSTION_NAME);
			BaseValueType minPos;
			BaseValueType maxPos;
			BaseValueType::length_type len = BaseValueType::length();
			for (BaseValueType::length_type i = 0; i < len; ++i)
			{
				typename BaseValueType::value_type min = std::numeric_limits<typename BaseValueType::value_type>::max(), max = -std::numeric_limits<typename BaseValueType::value_type>::max();
				for (uint32_t j = 0; i < m_vertexCount && j < static_cast<uint32_t>(vertices.size()); ++j)
				{
					if (min > vertices[j][i])min = vertices[j][i];
					if (max < vertices[j][i])max = vertices[j][i];
				}
				minPos[i] = min;
				maxPos[i] = max;
			}
			m_bounds.setMin(minPos);
			m_bounds.setMax(maxPos);
		}
	};
}

#endif // !KGS_MESH_H
