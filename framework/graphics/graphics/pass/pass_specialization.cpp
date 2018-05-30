#include "graphics/pass/pass_specialization.hpp"

#include "graphics/util/util.hpp"

namespace vg
{
    PassSpecializationData::SpecializationItem::SpecializationItem(uint32_t priority
        , std::vector<Byte> data
        , uint32_t size
        )
        : m_priority(priority)
        , m_data(data)
        , m_size(size)
    {}

    PassSpecializationData::SpecializationItem::SpecializationItem(uint32_t priority
        , const void *pData
        , uint32_t size
        )
        : m_priority(priority)
        , m_data()
        , m_size()
    {
        setData(pData, size);
    }

    PassSpecializationData::SpecializationItem::SpecializationItem(const SpecializationItem &target)
        : m_priority(target.m_priority)
        , m_data(target.m_data)
        , m_size(target.m_size)
    {

    }

    PassSpecializationData::SpecializationItem &PassSpecializationData::SpecializationItem::operator=(const SpecializationItem &target)
    {
        m_priority = target.m_priority;
        m_data = target.m_data;
        m_size = target.m_size;
        return *this;
    }

    void PassSpecializationData::SpecializationItem::setData(const void *pData
        , uint32_t size
        )
    {
        if (m_data.size() < size) {
            m_data.resize(size);
        }
        memcpy(m_data.data(), pData, size);
        m_size = size;
    }

    uint32_t PassSpecializationData::SpecializationItem::getPriority() const
    {
        return m_priority;
    }

    const void *PassSpecializationData::SpecializationItem::getData() const
    {
        return m_data.data();
    }
            
    uint32_t PassSpecializationData::SpecializationItem::getSize() const
    {
        return m_size;
    }

    PassSpecializationData::PassSpecializationData()
        : m_mapItems()
        , m_arrItemNames()
    {

    }

    Bool32 PassSpecializationData::hasSpecialization(std::string name) const
    {
        return hasValue(name, m_mapItems, m_arrItemNames);
    }

    void PassSpecializationData::addSpecialization(std::string name
        , uint32_t priority
        , const void *pData
        , uint32_t size
        )
    {
        SpecializationItem temp(priority, pData, size);
        addValue(name, temp, m_mapItems, m_arrItemNames);
    }

    void PassSpecializationData::removeSpecialization(std::string name)
    {
        removeValue(name, m_mapItems, m_arrItemNames);
    }

    const PassSpecializationData::SpecializationItem &PassSpecializationData::getSpecialization(std::string name) const
    {
        return getValue(name, m_mapItems, m_arrItemNames);
    }

    void PassSpecializationData::setSpecialization(std::string name
        , uint32_t priority
        , const void *pData
        , uint32_t size
        )
    {
        SpecializationItem temp(priority, pData, size);
        setValue(name, temp, m_mapItems, m_arrItemNames);
    }

    const std::vector<std::string> &PassSpecializationData::getSpecializationNames() const
    {
        return m_arrItemNames;
    }
} //vg