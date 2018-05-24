namespace vg
{
	template<TextureCoordinateType textureCoordinateType, TextureCoordinateIndex textureCoordinateIndex>
	const typename MeshData::DataTypeInfo<TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE>::ValueType &SepMesh::getTextureCoordinates() const
	{
		return getData<TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE>(TextureCoordinateIndexInfo<textureCoordinateIndex>::VERTEX_NAME);
	}

	template<TextureCoordinateType textureCoordinateType, TextureCoordinateIndex textureCoordinateIndex>
	void SepMesh::setTextureCoordinates(const typename MeshData::DataTypeInfo<TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE>::ValueType &textureCoordinates)
	{
		setData<TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE>(TextureCoordinateIndexInfo<textureCoordinateIndex>::VERTEX_NAME, textureCoordinates, TextureCoordinateIndexInfo<textureCoordinateIndex>::VERTEX_BINDING_PRIORITY);
	}

	template<MeshData::DataType dataType>
	Bool32 &SepMesh::hasData(std::string name) const
	{
		return m_pData->hasData<dataType>(name);
	}

	template<MeshData::DataType dataType>
	void &SepMesh::addData(std::string name
	    , const typename MeshData::DataTypeInfo<dataType>::ValueType &value
		, uint32_t bindingPriority = VG_VERTEX_BINDING_PRIORITY_OTHER_MIN
		)
	{
		//update layout binding info
		MeshData::DataInfo info = {
			name,
			dataType,
			bindingPriority
		};
		m_pData->addData<dataType>(name, info, value);
		m_applied = VG_FALSE;
	}

	template<MeshData::DataType dataType>
	void &SepMesh::removeData(std::string name) const
	{
		m_pData->removeData<dataType>(name);
		removeValue(name, m_mapLayoutBindingInfos, m_arrLayoutBindingInfoNames);
		m_applied = VG_FALSE;
	}

	template<MeshData::DataType dataType>
	const typename MeshData::DataTypeInfo<dataType>::ValueType &SepMesh::getData(std::string name) const
	{
		return m_pData->getData<dataType>(name);
	}

	template <MeshData::DataType dataType>
	void SepMesh::setData(std::string name, const typename MeshData::DataTypeInfo<dataType>::ValueType &value)
	{
		m_pData->setData<dataType>(name, value);
		m_applied = VG_FALSE;
	}

} //namespace vg