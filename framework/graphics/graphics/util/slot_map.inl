namespace vg
{
    template <typename T>
    SlotMap<T>::SlotMap()
        : m_slots()
        , m_map()
    {

    }
    
    template <typename T>
    Bool32 SlotMap<T>::hasValue(const lotType &slot) const
    {
        return m_map.count(slot) != 0;
    }

    template <typename T>
    void  SlotMap<T>::addValue(const SlotType &slot, const ValueType &value)
    {
        {
            const auto &iterator = m_map.find(slot);
            if (iterator != m_map.cend())
            {
                throw std::runtime_error("Map has exist the item whose key: " + name);
            }
            m_map.insert({slot, value});
        }
        {
            m_slots.insert(slot);
        }
    }

    template <typename T>
    void  SlotMap<T>::removeValue(const SlotType &slot)
    {
        m_map.erase(slot);
        m_slots.erase(slot);
    }

    template <typename T>
    SlotMap<T>::ValueType & SlotMap<T>::getValue(const SlotType &slot) const
    {
        const auto &iterator = m_map.find(slot);
        if (iterator == m_map.cend())
        {
            throw std::runtime_error("Map don't has item whose key: " + name);
        }
        else
        {
            return iterator->second;
        }
    }

    template <typename T>
    void  SlotMap<T>::setValue(const SlotType &slot, const ValueType &value)
    {
        {
            const auto &iterator = m_map.find(slot);
            if (iterator == m_map.cend())
            {
                throw std::runtime_error("Map don't has item whose key: " + name);
            }
            else
            {
                iterator->second = value;
            }
        }
    }

    template <typename T>
    const std::set<SlotType> &SlotMap<T>::getSlots() const
    {
        return m_slots;
    }
} //vg