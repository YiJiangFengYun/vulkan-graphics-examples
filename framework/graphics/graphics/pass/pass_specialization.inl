namespace vg
{
    template <typename T>
    PassSpecializationData::SpecializationItem::SpecializationItem(uint32_t priority
        , const T &data
        )
        : m_priority(priority)
        , m_data()
        , m_size()
    {
        setData(data);
    }

    template <typename T>
    void PassSpecializationData::SpecializationItem::setData(const T &data)
    {
        setData(&data, static_cast<uint32_t>(sizeof(T)));
    }

    template <typename T>
    T PassSpecializationData::SpecializationItem::getData() const
    {
        T temp;
        memcpy(&temp, m_data.data(), sizeof(T));
    }

    template<typename T>
    void PassSpecializationData::addSpecialization(std::string name
        , uint32_t priority
        , const T &data
        )
    {
        SpecializationItem temp(priority, data);
        addValue(name, temp, m_mapItems, m_arrItemNames);
    }

    template <typename T>
    void PassSpecializationData::setSpecialization(std::string name
        , uint32_t priority
        , const T &data
        )
    {
        SpecializationItem temp(priority, data);
        setValue(name, temp, m_mapItems, m_arrItemNames);
    }
} //vg