namespace vg
{
	template<TextureCoordinateType textureCoordinateType, TextureCoordinateIndex textureCoordinateIndex>
	const typename MeshData::DataTypeInfo<TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE>::ValueType &SepMesh::getTextureCoordinates() const
	{
		return _getData<TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE>(TextureCoordinateIndexInfo<textureCoordinateIndex>::VERTEX_NAME);
	}

	template<TextureCoordinateType textureCoordinateType, TextureCoordinateIndex textureCoordinateIndex>
	void SepMesh::setTextureCoordinates(const typename MeshData::DataTypeInfo<TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE>::ValueType &textureCoordinates)
	{
		_setData<TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE>(TextureCoordinateIndexInfo<textureCoordinateIndex>::VERTEX_NAME, textureCoordinates, TextureCoordinateIndexInfo<textureCoordinateIndex>::VERTEX_BINDING_PRIORITY);
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
} //namespace vg