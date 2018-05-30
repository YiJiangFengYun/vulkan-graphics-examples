namespace vg
{
    template<TextureCoordinateType textureCoordinateType, TextureCoordinateIndex textureCoordinateIndex>
    Bool32 SepMesh::hasTextureCoordinates() const
    {
        const auto TYPE = TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE;
        const auto SLOT = TextureCoordinateIndexInfo<textureCoordinateIndex>::SLOT;
        return hasData<TYPE>(SLOT);
    }

    template<TextureCoordinateType textureCoordinateType, TextureCoordinateIndex textureCoordinateIndex>
    void SepMesh::addTextureCoordinates(const typename MeshData::DataTypeInfo<TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE>::ValueType &textureCoordinates)
    {
        const auto TYPE = TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE;
        const auto SLOT = TextureCoordinateIndexInfo<textureCoordinateIndex>::SLOT;
        addData<TYPE>(SLOT, textureCoordinates);
    }

    template<TextureCoordinateType textureCoordinateType, TextureCoordinateIndex textureCoordinateIndex>
    void SepMesh::removeTextureCoordinates()
    {
        const auto TYPE = TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE;
        const auto SLOT = TextureCoordinateIndexInfo<textureCoordinateIndex>::SLOT;
        removeData<TYPE>(SLOT);
    }

    template<TextureCoordinateType textureCoordinateType, TextureCoordinateIndex textureCoordinateIndex>
    const typename MeshData::DataTypeInfo<TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE>::ValueType SepMesh::getTextureCoordinates() const
    {
        const auto TYPE = TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE;
        const auto SLOT = TextureCoordinateIndexInfo<textureCoordinateIndex>::SLOT;
        return getData<TYPE>(SLOT);
    }

    template<TextureCoordinateType textureCoordinateType, TextureCoordinateIndex textureCoordinateIndex>
    void SepMesh::setTextureCoordinates(const typename MeshData::DataTypeInfo<TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE>::ValueType &textureCoordinates)
    {
        const auto TYPE = TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE;
        const auto SLOT = TextureCoordinateIndexInfo<textureCoordinateIndex>::SLOT;
        setData<TYPE>(SLOT, textureCoordinates);
    }

    template<MeshData::DataType dataType>
    Bool32 SepMesh::hasData(const SlotType &slot) const
    {
        return m_pData->hasData<dataType>(slot);
    }

    template<MeshData::DataType dataType>
    void SepMesh::addData(const SlotType &slot
        , const typename MeshData::DataTypeInfo<dataType>::ValueType &value
        )
    {
        //update layout binding info
        MeshData::DataInfo info = {
            dataType,
            slot,
        };
        m_pData->addData<dataType>(slot, info, value);
        m_applied = VG_FALSE;
    }

    template<MeshData::DataType dataType>
    void SepMesh::removeData(const SlotType &slot)
    {
        m_pData->removeData<dataType>(slot);
        m_applied = VG_FALSE;
    }

    template<MeshData::DataType dataType>
    const typename MeshData::DataTypeInfo<dataType>::ValueType SepMesh::getData(const SlotType &slot) const
    {
        return m_pData->getData<dataType>(slot);
    }

    template <MeshData::DataType dataType>
    void SepMesh::setData(const SlotType &slot, const typename MeshData::DataTypeInfo<dataType>::ValueType &value)
    {
        m_pData->setData<dataType>(slot, value);
        m_applied = VG_FALSE;
    }

} //namespace vg