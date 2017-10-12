namespace kgs
{
	template <MeshType meshType>
	Mesh<meshType>::Mesh() 
		: m_pContext(pContext)
		, m_multipliedColor(COLOR_WHITE) //default multiplied color should be (1, 1, 1, 1)
	{

	}

	template <MeshType meshType>
	Mesh<meshType>::~Mesh()
	{

	}

	template <MeshType meshType>
	MeshType Mesh<meshType>::getMeshType() const
	{
		return m_meshType;
	}

	template <MeshType meshType>
	uint32_t Mesh<meshType>::getVertexCount() const
	{
		return m_vertexCount;
	}

	template <MeshType meshType>
	void Mesh<meshType>::setVertexCount(uint32_t value)
	{
		m_vertexCount = value;
	}

	template <MeshType meshType>
	typename Mesh<meshType>::ArrayValueType Mesh<meshType>::getVertices() const
	{
		return getData<Mesh<meshType>::ARRAY_DATA_TYPE>(KGS_VERTEX_POSITION_NAME);
	}

	template <MeshType meshType>
	void Mesh<meshType>::setVertices(ArrayValueType vertices)
	{
		setData<Mesh<meshType>::ARRAY_DATA_TYPE>(KGS_VERTEX_POSITION_NAME, vertices, KGS_VERTEX_BINDING_PRIORITY_POSITION);
	}

	template <MeshType meshType>
	std::vector<Color32> Mesh<meshType>::getColors() const
	{
		return getData<MeshData::DataType::COLOR_32_ARRAY>(KGS_VERTEX_COLOR_NAME);
	}

	template <MeshType meshType>
	void Mesh<meshType>::setColors(std::vector<Color32> colors)
	{
		setData<MeshData::DataType::COLOR_32_ARRAY>(KGS_VERTEX_COLOR_NAME, colors, KGS_VERTEX_BINDING_PRIORITY_COLOR);
	}

	template <MeshType meshType>
	typename Mesh<meshType>::ArrayValueType Mesh<meshType>::getNormals() const
	{
		getData<Mesh<meshType>::ARRAY_DATA_TYPE>(KGS_VERTEX_NORMAL_NAME);
	}

	template <MeshType meshType>
	void Mesh<meshType>::setNormals(ArrayValueType normals)
	{
		setData<Mesh<meshType>::ARRAY_DATA_TYPE>(KGS_VERTEX_NORMAL_NAME, normals, KGS_VERTEX_BINDING_PRIORITY_NORMAL);
	}

	template <MeshType meshType>
	typename Mesh<meshType>::ArrayValueType Mesh<meshType>::getTangents() const
	{
		getData<Mesh<meshType>::ARRAY_DATA_TYPE>(KGS_VERTEX_TANGENT_NAME);
	}

	template <MeshType meshType>
	void Mesh<meshType>::setTangents(ArrayValueType tangents)
	{
		setData<Mesh<meshType>::ARRAY_DATA_TYPE>(KGS_VERTEX_TANGENT_NAME, tangents, KGS_VERTEX_BINDING_PRIORITY_TANGENT);
	}

	template <MeshType meshType>
	template<UVType uvType, UVIndex uvIndex>
	typename MeshData::DataTypeInfo<UVConstInfo<uvType>::ARRAY_TYPE>::ValueType  Mesh<meshType>::getUVs()
	{
		return getData<UVConstInfo<uvType>::ARRAY_TYPE>(UVIndexInfo<uvIndex>::VERTEX_NAME);
	}

	template <MeshType meshType>
	template<UVType uvType, UVIndex uvIndex>
	void Mesh<meshType>::setUVs(typename MeshData::DataTypeInfo<UVConstInfo<uvType>::ARRAY_TYPE>::ValueType uvs, uint32_t uvIndex)
	{
		setData<UVConstInfo<uvType>::ARRAY_TYPE>(UVIndexInfo<uvIndex>::VERTEX_NAME, uvs, UVIndexInfo<uvIndex>::VERTEX_BINDING_PRIORITY);
	}

	template <MeshType meshType>
	template<MeshData::DataType dataType>
	typename MeshData::DataTypeInfo<dataType>::ValueType Mesh<meshType>::getData(std::string name)
	{
		return m_pData->getDataValue<dataType>(name);
	}

	template <MeshType meshType>
	template <MeshData::DataType dataType>
	void Mesh<meshType>::setData(std::string name, typename MeshData::DataTypeInfo<dataType>::ValueType value, uint32_t bindingPriority = KGS_VERTEX_BINDING_PRIORITY_OTHER_MIN)
	{
		m_pData->setDataValue<dataType>(name, value);
		//update layout binding info
		Mesh<meshType>::LayoutBindingInfo info(
			name,
			dataType,
			bindingPriority
		);
		setValue(name, info, m_mapLayoutBindingInfos, m_arrLayoutBindingInfos);
	}

	template <MeshType meshType>
	uint32_t Mesh<meshType>::getSubMeshCount() const
	{
		return m_subMeshCount;
	}

	template <MeshType meshType>
	void Mesh<meshType>::setSubMeshCount(uint32_t value)
	{
		m_subMeshCount = value;
		if (static_cast<uint32_t>(m_subMeshInfos.size()) < value)
		{
			m_subMeshInfos.resize(value);
		}
	}

	template <MeshType meshType>
	std::vector<uint32_t> Mesh<meshType>::getIndices(uint32_t subMeshIndex) const
	{
#ifdef DEBUG
		verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
		return m_subMeshInfos[subMeshIndex];
	}

	template <MeshType meshType>
	void Mesh<meshType>::setIndices(std::vector<uint32_t> indices, PrimitiveTopology topology, uint32_t subMeshIndex)
	{
#ifdef DEBUG
		verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
		SubMeshInfo& subMeshInfo = m_subMeshInfos[subMeshIndex];
		subMeshInfo.topology = topology;
		subMeshInfo.indices = indices;
	}

	template <MeshType meshType>
	uint32_t Mesh<meshType>::getIndexCount(uint32_t subMeshIndex) const
	{
#ifdef DEBUG
		verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
		return _getIndexCount(subMeshIndex);
	}

	template <MeshType meshType>
	uint32_t Mesh<meshType>::getIndexStart(uint32_t subMeshIndex) const
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

	template <MeshType meshType>
	PrimitiveTopology Mesh<meshType>::getTopology(uint32_t subMeshIndex) const
	{
#ifdef DEBUG
		verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
		return m_subMeshInfos[subMeshIndex].topology;
	}

	template <MeshType meshType>
	fd::Bounds<typename Mesh<meshType>::PointType> Mesh<meshType>::getBounds()
	{
		return m_bounds;
	}

	template <MeshType meshType>
	Color Mesh<meshType>::getMultipliedColor() const
	{
		return m_multipliedColor;
	}

	template <MeshType meshType>
	void Mesh<meshType>::setMultipliedColor(Color value)
	{
		m_multipliedColor = value;
	}

	template <MeshType meshType>
	Color Mesh<meshType>::getAddedColor() const
	{
		return m_addedColor;
	}

	template <MeshType meshType>
	void Mesh<meshType>::setAddedColor(Color value)
	{
		m_addedColor = value;
	}

	template <MeshType meshType>
	void Mesh<meshType>::apply(Bool32 makeUnreadable)
	{
		//sort layout binding infos
		_sortLayoutBindingInfos();

		//caculate bounds
		_updateBounds();

		//create vertex buffer
		_createVertexBuffer();

		//create index buffer
		_createIndexBuffer();
	}

	template <MeshType meshType>
	void Mesh<meshType>::_fillGraphicsPipelineCreateInfoForDraw(uint32_t subMeshIndex, vk::GraphicsPipelineCreateInfo &graphicsPipelineCreateInfo)
	{
		std::vector<vk::VertexInputBindingDescription> bindingDescriptions = _getVertexInputBindingDescriptions();
		std::vector<vk::VertexInputAttributeDescription> attributeDescriptions = _getVertexInputAttributeDescriptions();
		vk::PipelineVertexInputStateCreateInfo vertexInputStateInfo = {
			vk::PipelineVertexInputStateCreateFlags(),
			static_cast<uint32_t>(bindingDescriptions.size()),
			bindingDescriptions.data(),
			static_cast<uint32_t>(attributeDescriptions.size()),
			attributeDescriptions.data()
		};

		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo = {
			vk::PipelineInputAssemblyStateCreateFlags(),
			m_usingSubMeshInfos[subMeshIndex].topology,
			VK_FALSE
		};

		graphicsPipelineCreateInfo.pVertexInputState = &vertexInputStateInfo;
		graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateInfo;
	}

	template <MeshType meshType>
	void Mesh<meshType>::_fillCommandBufferForDraw(uint32_t subMeshIndex, vk::CommandBuffer &commandBuffer)
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
		commandBuffer.bindVertexBuffers(0u, vertexBuffers.data(), offsets.data());

		uint32_t offset = 0u;
		for (uint32_t i = 0; i < subMeshIndex; ++i)
		{
			std::vector<uint32_t>& indices = m_usingSubMeshInfos[i].indices;
			size_t size = indices.size() * sizeof(uint32_t);
			offset += static_cast<uint32_t>(size);
		}
		commandBuffer.bindIndexBuffer(m_pIndexBuffer, static_cast<vk:DeviceSize>(offset), vk::IndexType::eUint32);
	}

	template <MeshType meshType>
	inline void Mesh<meshType>::_sortLayoutBindingInfos()
	{
		m_layoutBindingInfos = std::set<LayoutBindingInfo>(m_arrLayoutBindingInfos.cbegin(), m_arrLayoutBindingInfos.cend(),
			[](LayoutBindingInfo lhs, LayoutBindingInfo rhs) {
			return lhs.bindingPriority < rhs.bindingPriority;
		});
	}

	template <MeshType meshType>
	inline std::vector<vk::VertexInputBindingDescription> Mesh<meshType>::_getVertexInputBindingDescriptions()
	{
		std::vector<vk::VertexInputBindingDescription> descriptions(m_layoutBindingInfos.size());
		uint32_t index = 0u;
		for (const auto& info : m_layoutBindingInfos)
		{
			descriptions[index].binding = index;
			descriptions[index].stride = MeshData::getDataBaseTypeSize(info.dataType) * m_vertexCount;
			descriptions[index].inputRate = vk::VertexInputRate::eVertex;
			++index;
		}
		return descriptions;
	}

	template <MeshType meshType>
	inline std::vector<vk::VertexInputAttributeDescription> Mesh<meshType>::_getVertexInputAttributeDescriptions()
	{
		std::vector<vk::VertexInputAttributeDescription> descriptions(m_layoutBindingInfos.size());
		uint32_t index = 0u;
		for (const auto& info : m_layoutBindingInfos)
		{
			descriptions[index].binding = index;
			descriptions[index].location = 0u;
			descriptions[index].format = MeshData::DataTypeInfo<info.dataType>::BASE_FORMAT;
			descriptions[index].offset = 0u;
			++index;
		}
		return descriptions;
	}

#ifdef DEBUG
	template <MeshType meshType>
	inline void Mesh<meshType>::verifySubMeshIndex(uint32_t subMeshIndex)
	{
		if (subMeshIndex >= m_subMeshCount)
			throw std::range_error("The subMeshIndex out of range of the actual sub mesh count.");
	}
#endif // DEBUG

	template <MeshType meshType>
	inline uint32_t Mesh<meshType>::_getIndexCount(uint32_t subMeshIndex) const
	{
		SubMeshInfo& subMeshInfo = m_subMeshInfos[subMeshIndex];
		return static_cast<uint32_t>(subMeshInfo.indices.size());
	}

	template <MeshType meshType>
	void Mesh<meshType>::_copyBuffer(std::shared_ptr<vk::Buffer>& pSrcBuffer, std::shared_ptr<vk::Buffer>& pDstBuffer, vk::DeviceSize size)
	{
		auto pCommandBuffer = beginSingleTimeCommands();

		vk::BufferCopy copyRegin = {};
		copyRegin.srcOffset = 0;
		copyRegin.dstOffset = 0;
		copyRegin.size = size;
		pCommandBuffer->copyBuffer(*pSrcBuffer, *pDstBuffer, copyRegin);

		endSingleTimeCommands(pCommandBuffer);
	}

	template <MeshType meshType>
	inline void Mesh<meshType>::_createVertexBuffer()
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

	template <MeshType meshType>
	inline void Mesh<meshType>::_createIndexBuffer()
	{
		m_usingSubMeshInfos = m_subMeshInfos;

		//get index buffer size
		uint32_t indexBufferSize = 0u;
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

	template <MeshType meshType>
	inline void Mesh<meshType>::_updateBounds()
	{
		if (m_vertexCount == 0u)
		{
			m_bounds = {};
			return;
		}

		auto vertices = m_pData->getDataValue<ARRAY_DATA_TYPE>(KGS_VERTEX_POSITION_NAME);
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
} //namespace kgs