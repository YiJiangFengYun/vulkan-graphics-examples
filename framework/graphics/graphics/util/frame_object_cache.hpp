#ifndef VG_FRAME_OBJECT_CACHE_HPP
#define VG_FRAME_OBJECT_CACHE_HPP

#include <functional>
#include <unordered_map>
#include "graphics/global.hpp"

namespace vg
{
    template <typename KeyType, typename ObjectType>
    class FrameObjectCache {
    public:
        using Creator = std::function<ObjectType(const KeyType &key)>;
        FrameObjectCache();
        FrameObjectCache(const Creator &creator);
        const Creator &getCreator() const;
        void setCreator(const Creator &creator);

        void begin();
        ObjectType caching(KeyType key);
        void end();
    private:
        Bool32 m_isDoing;
        std::unordered_map<KeyType, ObjectType> m_mapObjects;
        std::unordered_map<KeyType, ObjectType> m_mapOldObjects;
        Bool32 m_hasCreator;
        Creator m_creator;
    };
} // vg

#include "graphics/util/frame_object_cache.inl"

#endif //VG_FRAME_OBJECT_CACHE_HPP