namespace vg
{
    template<typename T>
    void PassPushConstantData::ConstantUpdate::set(uint32_t offset
        , const T &data
        )
    {
        setData(offset, data);
    }

    template <typename T>
    void PassPushConstantData::ConstantUpdate::setData(const T &data)
    {
        setData(&data, static_cast<uint32_t>(sizeof(T)));
    }

    template<typename T>
    T PassPushConstantData::ConstantUpdate::getData() const
    {
        T temp;
        memcpy(&temp, m_data.data(), sizeof(T));
    }

    template<typename T>
    void PassPushConstantData::setPushConstantUpdate(std::string name
        , uint32_t offset
        , const T &data
        )
    {
        auto &temp = getValue(name, m_mapItems, m_arrItemNames);
        temp.getUpdate().set(offset, data);
    }
} //vg