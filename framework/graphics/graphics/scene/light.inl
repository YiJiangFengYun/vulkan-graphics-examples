namespace vg
{
    template<typename T>
    void LightData::addData(const std::string name, const LightDataInfo &info, const T &value)
    {
        std::vector<Byte> temp(sizeof(T));
        memcpy(temp.data(), &value, sizeof(T));
        addValue(name, temp, mapDatas, arrDataNames);
        addValue(name, 1u, mapDataCounts);
        addValue(name, info, mapDataInfos);
        LightDataSizeInfo sizeInfo = {
            static_cast<uint32_t>(sizeof(T)),
        };
        addValue(name, sizeInfo, mapDataSizeInfos);
    }

    template <typename T>
    T LightData::getData(const std::string name) const
    {
        const auto& bytes = getValue(name, mapDatas);
        T t;
        memcpy(&t, bytes.data(), sizeof(T));
        return t;
    }

    template<typename T>
    void LightData::setData(const std::string name, const T &value)
    {
        std::vector<Byte> temp(sizeof(T));
        memcpy(temp.data(), &value, sizeof(T));
        setValue(name, temp, mapDatas, arrDataNames);
        setValue(name, 1u, mapDataCounts);
        LightDataSizeInfo sizeInfo = {
            static_cast<uint32_t>(sizeof(T)),
        };
        setValue(name, sizeInfo, mapDataSizeInfos);
    }

    template<typename T>
    void LightData::addData(const std::string name, const LightDataInfo &info, const std::vector<T> &values)
    {
        std::vector<Byte> temp(sizeof(T) * values.size());
        memcpy(temp.data(), values.data(), temp.size());
        addValue(name, temp, mapDatas, arrDataNames);
        addValue(name, values.size(), mapDataCounts);
        addValue(name, info, mapDataInfos);
        LightDataSizeInfo sizeInfo = {
            static_cast<uint32_t>(sizeof(T) * values.size()),
        };
        addValue(name, sizeInfo, mapDataSizeInfos);
    }

    template <typename T>
    std::vector<T> LightData::getData(const std::string name, const uint32_t count) const
    {
        const auto& bytes = getValue(name, mapDatas);
        std::vector<T> ts(count);
        memcpy(ts.data(), bytes.data(), sizeof(T) * count);
        return ts;
    }

    template<typename T>
    void LightData::setData(const std::string name, const std::vector<T> &values)
    {
        std::vector<Byte> temp(sizeof(T) * values.size());
        memcpy(temp.data(), values.data(), temp.size());
        setValue(name, temp, mapDatas, arrDataNames);
        setValue(name, values.size(), mapDataCounts);
        LightDataSizeInfo sizeInfo = {
            static_cast<uint32_t>(sizeof(T) * values.size()),
        };
        setValue(name, sizeInfo, mapDataSizeInfos);
    }

    template<typename T>
    void LightData::addData(const std::string name, const LightDataInfo &info, const T * const pSrc, const uint32_t count)
    {
        std::vector<Byte> temp(sizeof(T) * count);
        if (count != 0u && pSrc != nullptr) memcpy(temp.data(), pSrc, temp.size());
        addValue(name, temp, mapDatas, arrDataNames);
        addValue(name, count, mapDataCounts);
        addValue(name, info, mapDataInfos);
        LightDataSizeInfo sizeInfo =  {
            static_cast<uint32_t>(sizeof(T)) * count,
        };
        addValue(name, sizeInfo, mapDataSizeInfos);
    }

    template<typename T>
    void LightData::getData(const std::string name, const T * const pDst, const uint32_t count)
    {
        const auto& bytes = getValue(name, mapDatas);
        size_t size = sizeof(T) * count;
        memcpy(pDst, bytes.data(), size);
    }

    template<typename T>
    void LightData::setData(const std::string name, const T * const pSrc, const uint32_t count)
    {
        std::vector<Byte> temp(sizeof(T) * count);
        if (count != 0u && pSrc != nullptr) memcpy(temp.data(), pSrc, temp.size());
        setValue(name, temp, mapDatas, arrDataNames);
        setValue(name, count, mapDataCounts);
        LightDataSizeInfo sizeInfo =  {
            static_cast<uint32_t>(sizeof(T)) * count,
        };
        setValue(name, sizeInfo, mapDataSizeInfos);
    }


    template <const LightRegisterInfo &registerInfo>
    LightRegistrable<registerInfo>::LightRegistrable()
    {

    }

    // template <const LightRegisterInfo &registerInfo>
    // const LightRegisterInfo RegistrableLight<registerInfo>::registerInfo = registerInfo;


    template <SpaceType SPACE_TYPE>
    DimLight<SPACE_TYPE>::DimLight()
        : Object<SPACE_TYPE>()
        , BaseLight()
    {
        m_objectType = ObjectType::LIGHT;
    }

    template <SpaceType SPACE_TYPE>
    void DimLight<SPACE_TYPE>::_beginRender()
    {
        //sync transfrom data.
        auto matrix = m_pTransform->getMatrixLocalToWorld();
        if (m_data.hasData(VG_LIGHT_DATA_TRANSFORM_NAME) == VG_FALSE) {
            LightDataInfo dataInfo = {
                VG_LIGHT_DATA_TRANSFORM_LAYOUT_PRIORITY
            };
            m_data.addData(VG_LIGHT_DATA_TRANSFORM_NAME, dataInfo, matrix);
            m_dataChanged = VG_TRUE;
        } else {
            m_data.setData(VG_LIGHT_DATA_TRANSFORM_NAME, matrix);
        }
        m_dataContentChanged = VG_TRUE;
        m_dataContentChanges[VG_LIGHT_DATA_TRANSFORM_NAME] = VG_TRUE;
        _apply();


    }

    template <SpaceType SPACE_TYPE>
    void DimLight<SPACE_TYPE>::_endRender()
    {

    }

    template <SpaceType SPACE_TYPE, const LightRegisterInfo &registerInfo>
    Light<SPACE_TYPE, registerInfo>::Light()
        : DimLight<SPACE_TYPE>()
        , LightRegistrable<registerInfo>()
    {
        
    }

    template <SpaceType SPACE_TYPE, const LightRegisterInfo &registerInfo>
    LightExportInfo Light<SPACE_TYPE, registerInfo>::getExportInfo() const
    {
        LightExportInfo exportInfo;
        exportInfo.dataSize = static_cast<uint32_t>(m_dataMemoryBuffer.size());
        exportInfo.pData = static_cast<const void *>(m_dataMemoryBuffer.data());
        exportInfo.textureCount = static_cast<uint32_t>(m_textureInfos.size());
        exportInfo.pTextureInfos = m_textureInfos.data();
        return exportInfo;
    }

    
} //vg