namespace vg
{
    template<TextureCoordinateType textureCoordinateType, TextureCoordinateIndex textureCoordinateIndex>
    Bool32 SepMesh::hasTextureCoordinates() const
    {
        const auto TYPE = TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE;
        const auto NAME = TextureCoordinateIndexInfo<textureCoordinateIndex>::VERTEX_NAME;
        return hasData<TYPE>(NAME);
    }

    template<TextureCoordinateType textureCoordinateType, TextureCoordinateIndex textureCoordinateIndex>
    void SepMesh::addTextureCoordinates(const typename MeshData::DataTypeInfo<TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE>::ValueType &textureCoordinates)
    {
        const auto TYPE = TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE;
        const auto NAME = TextureCoordinateIndexInfo<textureCoordinateIndex>::VERTEX_NAME;
        const auto PRIORITY = TextureCoordinateIndexInfo<textureCoordinateIndex>::VERTEX_BINDING_PRIORITY;
        addData<TYPE>(NAME, textureCoordinates, PRIORITY);
    }

    template<TextureCoordinateType textureCoordinateType, TextureCoordinateIndex textureCoordinateIndex>
    void SepMesh::removeTextureCoordinates()
    {
        const auto TYPE = TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE;
        const auto NAME = TextureCoordinateIndexInfo<textureCoordinateIndex>::VERTEX_NAME;
        removeData<TYPE>(NAME);
    }

    template<TextureCoordinateType textureCoordinateType, TextureCoordinateIndex textureCoordinateIndex>
    const typename MeshData::DataTypeInfo<TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE>::ValueType SepMesh::getTextureCoordinates() const
    {
        const auto TYPE = TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE;
        const auto NAME = TextureCoordinateIndexInfo<textureCoordinateIndex>::VERTEX_NAME;
        return getData<TYPE>(vertexName);
    }

    template<TextureCoordinateType textureCoordinateType, TextureCoordinateIndex textureCoordinateIndex>
    void SepMesh::setTextureCoordinates(const typename MeshData::DataTypeInfo<TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE>::ValueType &textureCoordinates)
    {
        const auto TYPE = TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE;
        const auto NAME = TextureCoordinateIndexInfo<textureCoordinateIndex>::VERTEX_NAME;
        setData<TYPE>(NAME, textureCoordinates);
    }

    template<MeshData::DataType dataType>
    Bool32 SepMesh::hasData(std::string name) const
    {
        return m_pData->hasData<dataType>(name);
    }

    template<MeshData::DataType dataType>
    void SepMesh::addData(std::string name
        , const typename MeshData::DataTypeInfo<dataType>::ValueType &value
        , uint32_t bindingPriority
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
    void SepMesh::removeData(std::string name)
    {
        m_pData->removeData<dataType>(name);
        m_applied = VG_FALSE;
    }

    template<MeshData::DataType dataType>
    const typename MeshData::DataTypeInfo<dataType>::ValueType SepMesh::getData(std::string name) const
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