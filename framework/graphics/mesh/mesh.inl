namespace vg
{
	template<MeshDimType meshDimType>
	Mesh<meshDimType>::Mesh()
		: DimensionMesh()
		, m_meshDimType(meshDimType)
		, m_hasBounds(VG_FALSE)
		, m_bounds()
	{
	}

	template<MeshDimType meshDimType>
	MeshDimType Mesh<meshDimType>::getMeshDimType() const
	{
		return m_meshDimType;
	}

	template<MeshDimType meshDimType>
	Bool32 Mesh<meshDimType>::getIsHasBounds() const
	{
		return m_hasBounds;
	}

	template<MeshDimType meshDimType>
	fd::Bounds<typename Mesh<meshDimType>::PointType> Mesh<meshDimType>::getBounds() const
	{
		return m_bounds;
	}

	template<UVType uvType, UVIndex uvIndex>
	const typename MeshData::DataTypeInfo<UVConstInfo<uvType>::ARRAY_TYPE>::ValueType &SepMesh::getUVs() const
	{
		return _getData<UVConstInfo<uvType>::ARRAY_TYPE>(UVIndexInfo<uvIndex>::VERTEX_NAME);
	}

	template<UVType uvType, UVIndex uvIndex>
	void SepMesh::setUVs(const typename MeshData::DataTypeInfo<UVConstInfo<uvType>::ARRAY_TYPE>::ValueType &uvs)
	{
		_setData<UVConstInfo<uvType>::ARRAY_TYPE>(UVIndexInfo<uvIndex>::VERTEX_NAME, uvs, UVIndexInfo<uvIndex>::VERTEX_BINDING_PRIORITY);
	}

	template<MeshData::DataType dataType>
	const typename MeshData::DataTypeInfo<dataType>::ValueType &SepMesh::getData(std::string name) const
	{
		return _getData<dataType>(name);
	}

	template <MeshData::DataType dataType>
	void SepMesh::setData(std::string name, const typename MeshData::DataTypeInfo<dataType>::ValueType &value, uint32_t bindingPriority)
	{
		_setData<dataType>(name, value, bindingPriority);
	}

	template<MeshData::DataType dataType>
	const typename MeshData::DataTypeInfo<dataType>::ValueType &SepMesh::_getData(std::string name) const
	{
		return m_pData->getDataValue<dataType>(name);
	}

	template <MeshData::DataType dataType>
	void SepMesh::_setData(std::string name, const typename MeshData::DataTypeInfo<dataType>::ValueType &value, uint32_t bindingPriority)
	{
		m_pData->setDataValue<dataType>(name, value);
		//update layout binding info
		LayoutBindingInfo info(
			name,
			dataType,
			bindingPriority
		);
		setValue(name, info, m_mapLayoutBindingInfos, m_arrLayoutBindingInfoNames);
		m_applied = VG_FALSE;
	}

	void SepMesh::_sortLayoutBindingInfos()
	{
		m_layoutBindingInfos.clear();
		for (const auto& name : m_arrLayoutBindingInfoNames)
		{
			const auto& item = m_mapLayoutBindingInfos[name];
			m_layoutBindingInfos.insert(item);
		}
	}

#ifdef DEBUG
	void SepMesh::_verifySubMeshIndex(uint32_t subMeshIndex) const
	{
		if (subMeshIndex >= m_subMeshCount)
			throw std::range_error("The subMeshIndex out of range of the actual sub mesh count.");
	}
#endif // DEBUG

	uint32_t SepMesh::_getIndexCount(uint32_t subMeshIndex) const
	{
		SubMeshInfo subMeshInfo = m_subMeshInfos[subMeshIndex];
		return static_cast<uint32_t>(subMeshInfo.indices.size());
	}

	template <MeshDimType meshDimType>
	DimSepMesh<meshDimType>::DimSepMesh() 
		: SepMesh()
		, Mesh<meshDimType>()
	{
		m_hasBounds = VG_TRUE;
	}

	template <MeshDimType meshDimType>
	DimSepMesh<meshDimType>::DimSepMesh(MemoryPropertyFlags bufferMemoryPropertyFlags) 
		: SepMesh(bufferMemoryPropertyFlags)
		, Mesh<meshDimType>()
	{
		m_hasBounds = VG_TRUE;
	}

	template <MeshDimType meshDimType>
	DimSepMesh<meshDimType>::~DimSepMesh()
	{

	}

	template <MeshDimType meshDimType>
	const typename DimSepMesh<meshDimType>::ArrayValueType &DimSepMesh<meshDimType>::getPositions() const
	{
		return _getData<DimSepMesh<meshDimType>::ARRAY_DATA_TYPE>(VG_VERTEX_POSITION_NAME);
	}

	template <MeshDimType meshDimType>
	void DimSepMesh<meshDimType>::setPositions(const ArrayValueType &vertices)
	{
		_setData<DimSepMesh<meshDimType>::ARRAY_DATA_TYPE>(VG_VERTEX_POSITION_NAME, vertices, VG_VERTEX_BINDING_PRIORITY_POSITION);
	}

	template <MeshDimType meshDimType>
	const typename DimSepMesh<meshDimType>::ArrayValueType &DimSepMesh<meshDimType>::getNormals() const
	{
		return _getData<DimSepMesh<meshDimType>::ARRAY_DATA_TYPE>(VG_VERTEX_NORMAL_NAME);
	}

	template <MeshDimType meshDimType>
	void DimSepMesh<meshDimType>::setNormals(const ArrayValueType &normals)
	{
		_setData<DimSepMesh<meshDimType>::ARRAY_DATA_TYPE>(VG_VERTEX_NORMAL_NAME, normals, VG_VERTEX_BINDING_PRIORITY_NORMAL);
	}

	template <MeshDimType meshDimType>
	const typename DimSepMesh<meshDimType>::ArrayValueType &DimSepMesh<meshDimType>::getTangents() const
	{
		return _getData<DimSepMesh<meshDimType>::ARRAY_DATA_TYPE>(VG_VERTEX_TANGENT_NAME);
	}

	template <MeshDimType meshDimType>
	void DimSepMesh<meshDimType>::setTangents(const ArrayValueType &tangents)
	{
		_setData<DimSepMesh<meshDimType>::ARRAY_DATA_TYPE>(VG_VERTEX_TANGENT_NAME, tangents, VG_VERTEX_BINDING_PRIORITY_TANGENT);
	}

	template <MeshDimType meshDimType>
	void DimSepMesh<meshDimType>::apply(Bool32 makeUnreadable)
	{
		//caculate bounds
		_updateBounds();

		SepMesh::apply(makeUnreadable);
	}

	template <MeshDimType meshDimType>
	inline void DimSepMesh<meshDimType>::_updateBounds()
	{
		if (m_vertexCount == 0u)
		{
			m_bounds = {PointType(0), PointType(0)};
			return;
		}

		auto vertices = m_pData->getDataValue<ARRAY_DATA_TYPE>(VG_VERTEX_POSITION_NAME);
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

    template <MeshDimType meshDimType>
	DimSimpleMesh<meshDimType>::DimSimpleMesh()
	    : InternalContentMesh()
		, Mesh<meshDimType>()
	{

	}

	template <MeshDimType meshDimType>
	DimSimpleMesh<meshDimType>::DimSimpleMesh(MemoryPropertyFlags bufferMemoryPropertyFlags)
	    : InternalContentMesh(bufferMemoryPropertyFlags)
		, Mesh<meshDimType>()
	{

	}

	template <MeshDimType meshDimType>
	void DimSimpleMesh<meshDimType>::setIsHasBounds(Bool32 isHasBounds)
	{
		m_hasBounds = isHasBounds;
	}

    template <MeshDimType meshDimType>
	void DimSimpleMesh<meshDimType>::setBounds(fd::Bounds<PointType> bounds)
	{
		m_bounds = bounds;
	}

    template <MeshDimType meshDimType>
	DimSharedContentMesh<meshDimType>::DimSharedContentMesh()
	    : ExternalContentMesh()
		, Mesh<meshDimType>()
	{

	}

	template <MeshDimType meshDimType>
	DimSharedContentMesh<meshDimType>::DimSharedContentMesh(std::shared_ptr<VertexData> pVertexData
		    , std::shared_ptr<IndexData> pIndexData
			, uint32_t subIndexDataOffset
			, uint32_t subIndexDataCount
			)
	{

	}

	template <MeshDimType meshDimType>
	void DimSharedContentMesh<meshDimType>::setIsHasBounds(Bool32 isHasBounds)
	{
		m_hasBounds = isHasBounds;
	}

    template <MeshDimType meshDimType>
	void DimSharedContentMesh<meshDimType>::setBounds(fd::Bounds<PointType> bounds)
	{
		m_bounds = bounds;
	}
} //namespace kgs