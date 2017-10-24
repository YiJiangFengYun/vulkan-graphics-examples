#include "graphics/mesh/mesh.hpp"

namespace kgs
{
	BaseMesh::LayoutBindingInfo::LayoutBindingInfo()
	{

	}

	BaseMesh::LayoutBindingInfo::LayoutBindingInfo(std::string name,
		MeshData::DataType dataType,
		uint32_t bindingPriority)
		: name(name)
		, dataType(dataType)
		, bindingPriority(bindingPriority)
	{

	}

	BaseMesh::LayoutBindingInfo::LayoutBindingInfo(const LayoutBindingInfo &target)
		: name(target.name)
		, dataType(target.dataType)
		, bindingPriority(target.bindingPriority)
	{

	}

	BaseMesh::LayoutBindingInfo &BaseMesh::LayoutBindingInfo::operator=(const LayoutBindingInfo &target)
	{
		name = target.name;
		dataType = target.dataType;
		bindingPriority = target.bindingPriority;
		return *this;
	}


	BaseMesh::LayoutBindingInfo::LayoutBindingInfo(const LayoutBindingInfo &&target)
		: name(target.name)
		, dataType(target.dataType)
		, bindingPriority(target.bindingPriority)
	{

	}

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
		: m_pContext(pContext)
		, m_multipliedColor(COLOR_WHITE) //default multiplied color should be (1, 1, 1, 1)
		, m_applied(KGS_FALSE)
	{
		_createMeshData();
		setSubMeshCount(1u);
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
		m_applied = KGS_FALSE;
	}

	const std::vector<Color32> &BaseMesh::getColors() const
	{
		return _getData<MeshData::DataType::COLOR_32_ARRAY>(KGS_VERTEX_COLOR_NAME);
	}

	void BaseMesh::setColors(const std::vector<Color32> &colors)
	{
		_setData<MeshData::DataType::COLOR_32_ARRAY>(KGS_VERTEX_COLOR_NAME, colors, KGS_VERTEX_BINDING_PRIORITY_COLOR);
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
		m_applied = KGS_FALSE;
	}

	const std::vector<uint32_t> &BaseMesh::getIndices(uint32_t subMeshIndex) const
	{
#ifdef DEBUG
		_verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
		return m_subMeshInfos[subMeshIndex].indices;
	}

	void BaseMesh::setIndices(const std::vector<uint32_t> &indices, PrimitiveTopology topology, uint32_t subMeshIndex)
	{
#ifdef DEBUG
		_verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
		SubMeshInfo& subMeshInfo = m_subMeshInfos[subMeshIndex];
		subMeshInfo.topology = topology;
		subMeshInfo.indices = indices;
		m_applied = KGS_FALSE;
	}

	uint32_t BaseMesh::getIndexCount(uint32_t subMeshIndex) const
	{
#ifdef DEBUG
		_verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
		return _getIndexCount(subMeshIndex);
	}

	uint32_t BaseMesh::getIndexStart(uint32_t subMeshIndex) const
	{
#ifdef DEBUG
		_verifySubMeshIndex(subMeshIndex);
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
		_verifySubMeshIndex(subMeshIndex);
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
		m_applied = KGS_FALSE;
	}

	Color BaseMesh::getAddedColor() const
	{
		return m_addedColor;
	}

	void BaseMesh::setAddedColor(Color value)
	{
		m_addedColor = value;
		m_applied = KGS_FALSE;
	}

	void BaseMesh::apply(Bool32 makeUnreadable)
	{
		if (m_applied == KGS_FALSE)
		{
			//sort layout binding infos
			_sortLayoutBindingInfos();

			//create vertex buffer
			_createVertexBuffer();

			//create index buffer
			_createIndexBuffer();

			m_applied = KGS_TRUE;
		}

		if (makeUnreadable)
		{
			m_vertexCount = 0u;

			//clear data with new mesh data.
			_createMeshData();
			//clear layout binding info array with reallocate.
			std::vector<std::string>().swap(m_arrLayoutBindingInfoNames);
			//clear layout binding info map with reallocate.
			std::unordered_map<std::string, LayoutBindingInfo>().swap(m_mapLayoutBindingInfos);
			//clear sub mesh info with reallocate.
			std::vector<SubMeshInfo>().swap(m_subMeshInfos);
			//reset sub mesh count to 1.
			setSubMeshCount(1u);
		}
	}

	vk::PrimitiveTopology BaseMesh::_getVKTopology(uint32_t subMeshIndex)
	{
		return tranPrimitiveTopologyTypeToVK(m_usingSubMeshInfos[subMeshIndex].topology);
	}

	void BaseMesh::_fillCommandBufferForDraw(uint32_t subMeshIndex, vk::CommandBuffer &commandBuffer)
	{
		std::vector<vk::Buffer> vertexBuffers(m_layoutBindingInfos.size());
		std::vector<vk::DeviceSize> offsets(m_layoutBindingInfos.size());
		uint32_t offset = 0u;
		uint32_t index = 0u;
		for (const auto& layoutInfo : m_layoutBindingInfos)
		{
			vertexBuffers[index] = *m_pVertexBuffer;
			offsets[index] = offset;
			offset += MeshData::getDataBaseTypeSize(layoutInfo.dataType) * m_vertexCount;
			++index;
		}

		commandBuffer.bindVertexBuffers(0u, vertexBuffers, offsets);

		offset = 0u;
		for (uint32_t i = 0; i < subMeshIndex; ++i)
		{
			std::vector<uint32_t>& indices = m_usingSubMeshInfos[i].indices;
			size_t size = indices.size() * sizeof(uint32_t);
			offset += static_cast<uint32_t>(size);
		}
		commandBuffer.bindIndexBuffer(*m_pIndexBuffer, static_cast<vk::DeviceSize>(offset), vk::IndexType::eUint32);
	}

	void BaseMesh::_createMeshData()
	{
		m_pData = std::shared_ptr<MeshData>(new MeshData());
	}

	void BaseMesh::_createVertexBuffer()
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