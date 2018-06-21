namespace vg
{
    template <typename KeyType, typename ObjectType>
    FrameObjectCache<KeyType, ObjectType>::FrameObjectCache()
        : m_isDoing()
        , m_mapObjects()
        , m_mapOldObjects()
    {

    }

    template <typename KeyType, typename ObjectType>
    void FrameObjectCache<KeyType, ObjectType>::begin()
    {
        m_isDoing = VG_TRUE;
        m_mapOldObjects = m_mapObjects;
        m_mapObjects.clear();
    }
    
    template <typename KeyType, typename ObjectType>
    void FrameObjectCache<KeyType, ObjectType>::end()
    {
        m_isDoing = VG_FALSE;
        m_mapOldObjects.clear();
    }

    template <typename KeyType, typename ObjectType>
    Bool32 FrameObjectCache<KeyType, ObjectType>::hasOldObject(KeyType key)
    {
        return m_mapOldObjects.count(key) != 0;
    }

    template <typename KeyType, typename ObjectType>
    ObjectType FrameObjectCache<KeyType, ObjectType>::getOldObject(KeyType key)
    {
        auto iterator = m_mapOldObjects.find(key);
        if (iterator == m_mapOldObjects.cend())
        {
            throw std::runtime_error("Don't has special old object.");
        }
        else
        {
            return iterator->second;
        }
    }
} //vg