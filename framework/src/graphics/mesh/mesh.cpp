#include "graphics/mesh/mesh.hpp"

namespace kgs
{
	Bool32 BaseMesh::LayoutBindingInfo::operator ==(const LayoutBindingInfo &target) const
	{
		return name == target.name && dataType == target.dataType && bindingPriority == target.bindingPriority;
	}

	Bool32 BaseMesh::LayoutBindingInfo::operator<(const LayoutBindingInfo &target) const
	{
		return bindingPriority < target.bindingPriority;
	}

	MeshType BaseMesh::getMeshType() const
	{
		return m_meshType;
	}

	BaseMesh::BaseMesh()
	{

	}

	BaseMesh::~BaseMesh()
	{

	}

	uint32_t BaseMesh::getVertexCount() const
	{
		return m_vertexCount;
	}

	void BaseMesh::setVertexCount(uint32_t value)
	{
		m_vertexCount = value;
	}

	std::vector<Color32> BaseMesh::getColors() const
	{
		return getData<MeshData::DataType::COLOR_32_ARRAY>(KGS_VERTEX_COLOR_NAME);
	}

	void BaseMesh::setColors(std::vector<Color32> colors)
	{
		setData<MeshData::DataType::COLOR_32_ARRAY>(KGS_VERTEX_COLOR_NAME, colors, KGS_VERTEX_BINDING_PRIORITY_COLOR);
	}

	uint32_t BaseMesh::getSubMeshCount() const
	{
		return m_subMeshCount;
	}

	void BaseMesh::setSubMeshCount(uint32_t value)
	{
		m_subMeshCount = value;
		if (static_cast<uint32_t>(m_subMeshInfos.size()) < value)
		{
			m_subMeshInfos.resize(value);
		}
	}

	std::vector<uint32_t> BaseMesh::getIndices(uint32_t subMeshIndex) const
	{
#ifdef DEBUG
		verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
		return m_subMeshInfos[subMeshIndex].indices;
	}

	void BaseMesh::setIndices(std::vector<uint32_t> indices, PrimitiveTopology topology, uint32_t subMeshIndex)
	{
#ifdef DEBUG
		verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
		SubMeshInfo& subMeshInfo = m_subMeshInfos[subMeshIndex];
		subMeshInfo.topology = topology;
		subMeshInfo.indices = indices;
	}

	uint32_t BaseMesh::getIndexCount(uint32_t subMeshIndex) const
	{
#ifdef DEBUG
		verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
		return _getIndexCount(subMeshIndex);
	}

	uint32_t BaseMesh::getIndexStart(uint32_t subMeshIndex) const
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

	PrimitiveTopology BaseMesh::getTopology(uint32_t subMeshIndex) const
	{
#ifdef DEBUG
		verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
		return m_subMeshInfos[subMeshIndex].topology;
	}

	Color BaseMesh::getMultipliedColor() const
	{
		return m_multipliedColor;
	}

	void BaseMesh::setMultipliedColor(Color value)
	{
		m_multipliedColor = value;
	}

	Color BaseMesh::getAddedColor() const
	{
		return m_addedColor;
	}

	void BaseMesh::setAddedColor(Color value)
	{
		m_addedColor = value;
	}

	void BaseMesh::apply(Bool32 makeUnreadable)
	{
		//sort layout binding infos
		_sortLayoutBindingInfos();

		//create vertex buffer
		_createVertexBuffer();

		//create index buffer
		_createIndexBuffer();
	}

	template<UVType uvType, UVIndex uvIndex>
	typename MeshData::DataTypeInfo<UVConstInfo<uvType>::ARRAY_TYPE>::ValueType  BaseMesh::getUVs()
	{
		return getData<UVConstInfo<uvType>::ARRAY_TYPE>(UVIndexInfo<uvIndex>::VERTEX_NAME);
	}

	template<UVType uvType, UVIndex uvIndex>
	void BaseMesh::setUVs(typename MeshData::DataTypeInfo<UVConstInfo<uvType>::ARRAY_TYPE>::ValueType uvs, uint32_t uvIndex)
	{
		setData<UVConstInfo<uvType>::ARRAY_TYPE>(UVIndexInfo<uvIndex>::VERTEX_NAME, uvs, UVIndexInfo<uvIndex>::VERTEX_BINDING_PRIORITY);
	}

	template<MeshData::DataType dataType>
	typename MeshData::DataTypeInfo<dataType>::ValueType BaseMesh::getData(std::string name) const
	{
		return m_pData->getDataValue<dataType>(name);
	}

	template <MeshData::DataType dataType>
	void BaseMesh::setData(std::string name, typename MeshData::DataTypeInfo<dataType>::ValueType value, uint32_t bindingPriority)
	{
		m_pData->setDataValue<dataType>(name, value);
		//update layout binding info
		LayoutBindingInfo info(
			name,
			dataType,
			bindingPriority
		);
		setValue(name, info, m_mapLayoutBindingInfos, m_arrLayoutBindingInfos);
	}

	inline void BaseMesh::_createVertexBuffer()
	{
		//get size of every vertex
		uint32_t size = 0u;
		for (const auto& layoutInfo : m_layoutBindingInfos)
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

		auto pPhysicalDevice = m_pContext->getPhysicalDevice();
		auto pDevice = m_pContext->getNativeDevice();
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
		uint32_t offset = 0u;
		for (const auto& layoutInfo : m_layoutBindingInfos)
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

	void BaseMesh::_createIndexBuffer()
	{
		m_usingSubMeshInfos = m_subMeshInfos;

		//get index buffer size
		uint32_t indexBufferSize = 0u;
		for (const auto& item : m_usingSubMeshInfos)
		{
			const std::vector<uint32_t>& indices = item.indices;
			indexBufferSize += static_cast<uint32_t>(indices.size() * sizeof(uint32_t));
		}

		//create staging buffer.
		vk::BufferCreateInfo createInfo = {
			vk::BufferCreateFlags(),
			indexBufferSize,
			vk::BufferUsageFlagBits::eTransferSrc,
			vk::SharingMode::eExclusive
		};

		auto pPhysicalDevice = m_pContext->getPhysicalDevice();
		auto pDevice = m_pContext->getNativeDevice();
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
		uint32_t offset = 0u;
		for (const auto& subMeshInfo : m_usingSubMeshInfos)
		{
			const std::vector<uint32_t>& indices = subMeshInfo.indices;
			size_t size = indices.size() * sizeof(uint32_t);
			memcpy((char*)data + offset, indices.data(), size);
			offset += static_cast<uint32_t>(size);
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
}