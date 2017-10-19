namespace kgs
{
	void BaseMesh::_fillGraphicsPipelineCreateInfoForDraw(uint32_t subMeshIndex, vk::GraphicsPipelineCreateInfo &graphicsPipelineCreateInfo)
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
			tranPrimitiveTopologyTypeToVK(m_usingSubMeshInfos[subMeshIndex].topology),
			VK_FALSE
		};

		graphicsPipelineCreateInfo.pVertexInputState = &vertexInputStateInfo;
		graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateInfo;
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

	void BaseMesh::_sortLayoutBindingInfos()
	{
		m_layoutBindingInfos = { m_arrLayoutBindingInfos.cbegin(), m_arrLayoutBindingInfos.cend() };
	}

#ifdef DEBUG
	void BaseMesh::verifySubMeshIndex(uint32_t subMeshIndex) const
	{
		if (subMeshIndex >= m_subMeshCount)
			throw std::range_error("The subMeshIndex out of range of the actual sub mesh count.");
	}
#endif // DEBUG

	uint32_t BaseMesh::_getIndexCount(uint32_t subMeshIndex) const
	{
		SubMeshInfo subMeshInfo = m_subMeshInfos[subMeshIndex];
		return static_cast<uint32_t>(subMeshInfo.indices.size());
	}

	void BaseMesh::_copyBuffer(std::shared_ptr<vk::Buffer>& pSrcBuffer, std::shared_ptr<vk::Buffer>& pDstBuffer, vk::DeviceSize size)
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
	Mesh<meshType>::Mesh() 
		: BaseMesh()
	{
		m_meshType = meshType;
	}

	template <MeshType meshType>
	Mesh<meshType>::~Mesh()
	{

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
	fd::Bounds<typename Mesh<meshType>::PointType> Mesh<meshType>::getBounds()
	{
		return m_bounds;
	}

	template <MeshType meshType>
	void Mesh<meshType>::apply(Bool32 makeUnreadable)
	{
		BaseMesh::apply(makeUnreadable);

		//caculate bounds
		_updateBounds();
	}

	template <MeshType meshType>
	std::vector<vk::VertexInputBindingDescription> Mesh<meshType>::_getVertexInputBindingDescriptions()
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
	std::vector<vk::VertexInputAttributeDescription> Mesh<meshType>::_getVertexInputAttributeDescriptions()
	{
		std::vector<vk::VertexInputAttributeDescription> descriptions(m_layoutBindingInfos.size());
		uint32_t index = 0u;
		for (const auto& info : m_layoutBindingInfos)
		{
			descriptions[index].binding = index;
			descriptions[index].location = 0u;
			descriptions[index].format = MeshData::getBaseFormatWithDataType(info.dataType);
			descriptions[index].offset = 0u;
			++index;
		}
		return descriptions;
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
			for (uint32_t j = 0; j < m_vertexCount && j < static_cast<uint32_t>(vertices.size()); ++j)
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