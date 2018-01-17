namespace kgs
{
	template<UVType uvType, UVIndex uvIndex>
	const typename MeshData::DataTypeInfo<UVConstInfo<uvType>::ARRAY_TYPE>::ValueType &BaseMesh::getUVs() const
	{
		return _getData<UVConstInfo<uvType>::ARRAY_TYPE>(UVIndexInfo<uvIndex>::VERTEX_NAME);
	}

	template<UVType uvType, UVIndex uvIndex>
	void BaseMesh::setUVs(const typename MeshData::DataTypeInfo<UVConstInfo<uvType>::ARRAY_TYPE>::ValueType &uvs)
	{
		_setData<UVConstInfo<uvType>::ARRAY_TYPE>(UVIndexInfo<uvIndex>::VERTEX_NAME, uvs, UVIndexInfo<uvIndex>::VERTEX_BINDING_PRIORITY);
	}

	template<MeshData::DataType dataType>
	const typename MeshData::DataTypeInfo<dataType>::ValueType &BaseMesh::getData(std::string name) const
	{
		return _getData<dataType>(name);
	}

	template <MeshData::DataType dataType>
	void BaseMesh::setData(std::string name, const typename MeshData::DataTypeInfo<dataType>::ValueType &value, uint32_t bindingPriority)
	{
		_setData<dataType>(name, value, bindingPriority);
	}

	template<MeshData::DataType dataType>
	const typename MeshData::DataTypeInfo<dataType>::ValueType &BaseMesh::_getData(std::string name) const
	{
		return m_pData->getDataValue<dataType>(name);
	}

	template <MeshData::DataType dataType>
	void BaseMesh::_setData(std::string name, const typename MeshData::DataTypeInfo<dataType>::ValueType &value, uint32_t bindingPriority)
	{
		m_pData->setDataValue<dataType>(name, value);
		//update layout binding info
		LayoutBindingInfo info(
			name,
			dataType,
			bindingPriority
		);
		setValue(name, info, m_mapLayoutBindingInfos, m_arrLayoutBindingInfoNames);
		m_applied = KGS_FALSE;
	}

	void BaseMesh::_sortLayoutBindingInfos()
	{
		m_layoutBindingInfos.clear();
		for (const auto& name : m_arrLayoutBindingInfoNames)
		{
			const auto& item = m_mapLayoutBindingInfos[name];
			m_layoutBindingInfos.insert(item);
		}
	}

#ifdef DEBUG
	void BaseMesh::_verifySubMeshIndex(uint32_t subMeshIndex) const
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
	const typename Mesh<meshType>::ArrayValueType &Mesh<meshType>::getPositions() const
	{
		return _getData<Mesh<meshType>::ARRAY_DATA_TYPE>(KGS_VERTEX_POSITION_NAME);
	}

	template <MeshType meshType>
	void Mesh<meshType>::setPositions(const ArrayValueType &vertices)
	{
		_setData<Mesh<meshType>::ARRAY_DATA_TYPE>(KGS_VERTEX_POSITION_NAME, vertices, KGS_VERTEX_BINDING_PRIORITY_POSITION);
	}

	template <MeshType meshType>
	const typename Mesh<meshType>::ArrayValueType &Mesh<meshType>::getNormals() const
	{
		_getData<Mesh<meshType>::ARRAY_DATA_TYPE>(KGS_VERTEX_NORMAL_NAME);
	}

	template <MeshType meshType>
	void Mesh<meshType>::setNormals(const ArrayValueType &normals)
	{
		_setData<Mesh<meshType>::ARRAY_DATA_TYPE>(KGS_VERTEX_NORMAL_NAME, normals, KGS_VERTEX_BINDING_PRIORITY_NORMAL);
	}

	template <MeshType meshType>
	const typename Mesh<meshType>::ArrayValueType &Mesh<meshType>::getTangents() const
	{
		_getData<Mesh<meshType>::ARRAY_DATA_TYPE>(KGS_VERTEX_TANGENT_NAME);
	}

	template <MeshType meshType>
	void Mesh<meshType>::setTangents(const ArrayValueType &tangents)
	{
		_setData<Mesh<meshType>::ARRAY_DATA_TYPE>(KGS_VERTEX_TANGENT_NAME, tangents, KGS_VERTEX_BINDING_PRIORITY_TANGENT);
	}

	template <MeshType meshType>
	fd::Bounds<typename Mesh<meshType>::PointType> Mesh<meshType>::getBounds()
	{
		return m_bounds;
	}

	template <MeshType meshType>
	void Mesh<meshType>::apply(Bool32 makeUnreadable)
	{
		//caculate bounds
		_updateBounds();

		BaseMesh::apply(makeUnreadable);
	}

	template <MeshType meshType>
	std::vector<vk::VertexInputBindingDescription> Mesh<meshType>::_getVertexInputBindingDescriptionsForRender()
	{
		std::vector<vk::VertexInputBindingDescription> descriptions(m_layoutBindingInfos.size());
		uint32_t index = 0u;
		for (const auto& info : m_layoutBindingInfos)
		{
			descriptions[index].binding = index;
			descriptions[index].stride = MeshData::getDataBaseTypeSize(info.dataType);
			descriptions[index].inputRate = vk::VertexInputRate::eVertex;
			++index;
		}
		return descriptions;
	}

	template <MeshType meshType>
	std::vector<vk::VertexInputAttributeDescription> Mesh<meshType>::_getVertexInputAttributeDescriptionsForRender()
	{
		std::vector<vk::VertexInputAttributeDescription> descriptions(m_layoutBindingInfos.size());
		uint32_t index = 0u;
		for (const auto& info : m_layoutBindingInfos)
		{
			descriptions[index].binding = index;
			descriptions[index].location = index;
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
		m_bounds.setMinMax(minPos, maxPos);
	}
} //namespace kgs