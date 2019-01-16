namespace vg
{
    template<typename T>
    void Pass::addData(const std::string name, const PassDataInfo &info, const T &value)
    {
        m_data.addData<T>(name, info, value);
        m_dataChanged = VG_TRUE;
    }

    template<typename T>
    T Pass::getData(const std::string name) const
    {
        return m_data.getData<T>(name);
    }

    template<typename T>
    void Pass::setData(const std::string name, const T &value)
    {
        m_data.setData<T>(name, value);
        m_dataContentChanged = VG_TRUE;
        m_dataContentChanges[name] = VG_TRUE;
    }
    
    template<typename T>
    void Pass::addData(const std::string name, const PassDataInfo &info, const std::vector<T> &values)
    {
        m_data.addData<T>(name, info, values);
        m_dataChanged = VG_TRUE;
    }

    template <typename T>
    std::vector<T> Pass::getData(const std::string name, const uint32_t count) const
    {
        return m_data.getData<T>(name, count);
    }

    template<typename T>
    void Pass::setData(const std::string name, const std::vector<T> &values)
    {
        m_data.setData<T>(name, values);
        m_dataContentChanged = VG_TRUE;
        m_dataContentChanges[name] = VG_TRUE;
    }

    template<typename T>
    void Pass::addData(const std::string name, const PassDataInfo &info, const T * const pSrc, const uint32_t count)
    {
        m_data.addData<T>(name, info, pSrc, count);
        m_dataChanged = VG_TRUE;
    }

    template<typename T>
    void Pass::getData(const std::string name, const T * const pDst, const uint32_t count)
    {
        m_data.getData<T>(name, pDst, count);
    }

    template<typename T>
    void Pass::setData(const std::string name, const T * const pSrc, const uint32_t count)
    {
        m_data.setData<T>(name, pSrc, count);
        m_dataContentChanged = VG_TRUE;
        m_dataContentChanges[name] = VG_TRUE;
    }

    template <typename T>
    void Pass::addSpecializationItem(vk::ShaderStageFlagBits shaderStage
        , std::string name
        , uint32_t priority
        , const T &data
        )
    {
        m_mapSpecilizationDatas[shaderStage].addSpecialization(name, priority, data);
        m_specializationChanged = VG_TRUE;
        m_mapSpecializationDataChanges[shaderStage] = VG_TRUE;
    }

    template <typename T>
    void Pass::setSpecializationItem(vk::ShaderStageFlagBits shaderStage
        , std::string name
        , uint32_t priority
        , const T &data
        )
    {
        m_mapSpecilizationDatas[shaderStage].setSpecialization(name, priority, data);
        m_specializationChanged = VG_TRUE;
        m_mapSpecializationDataChanges[shaderStage] = VG_TRUE;
    }

    template<typename T>
    void Pass::setPushConstantUpdate(std::string name
        , uint32_t offset
        , const T &data
        )
    {
        m_pushConstant.setPushConstantUpdate(name, offset, data);
    }
    
} //namespace kgs
