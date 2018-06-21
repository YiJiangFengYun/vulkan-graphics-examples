namespace vg
{
    template <typename KeyType, typename ObjectType>
    FrameObjectCache<KeyType, ObjectType>::FrameObjectCache()
        : m_isDoing()
        , m_mapObjects()
        , m_mapOldObjects()
        , m_hasCreator()
        , m_creator()
    {

    }

    template <typename KeyType, typename ObjectType>
    FrameObjectCache<KeyType, ObjectType>::FrameObjectCache(const Creator &creator)
        : FrameObjectCache()
    {
        m_hasCreator = VG_TRUE;
        m_creator = creator;
    }

    template <typename KeyType, typename ObjectType>
    const typename FrameObjectCache<KeyType, ObjectType>::Creator &FrameObjectCache<KeyType, ObjectType>::getCreator() const
    {
        return m_creator;
    }

    template <typename KeyType, typename ObjectType>
    void FrameObjectCache<KeyType, ObjectType>::setCreator(const Creator &creator)
    {
        m_hasCreator = VG_TRUE;
        m_creator = creator;
    }

    template <typename KeyType, typename ObjectType>
    void FrameObjectCache<KeyType, ObjectType>::begin()
    {
        m_isDoing = VG_TRUE;
        m_mapOldObjects = m_mapObjects;
        m_mapObjects.clear();
    }

    template <typename KeyType, typename ObjectType>
    ObjectType FrameObjectCache<KeyType, ObjectType>::caching(KeyType key)
    {
        Bool32 isHas = m_mapOldObjects.count(key) != 0;
        if (isHas)
        {
            auto iterator = m_mapOldObjects.find(key);
            auto oldObj = iterator->second;
            m_mapObjects.insert({key, oldObj});
            return oldObj;
        }
        else
        {
            ObjectType newObj;
            if (m_hasCreator) {
                newObj = m_creator(key);
            }
            m_mapObjects.insert({key, newObj});
            return newObj;
        }
    }
    
    template <typename KeyType, typename ObjectType>
    void FrameObjectCache<KeyType, ObjectType>::end()
    {
        m_isDoing = VG_FALSE;
        m_mapOldObjects.clear();
    }
} //vg