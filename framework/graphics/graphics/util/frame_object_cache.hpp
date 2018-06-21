#ifndef VG_FRAME_OBJECT_CACHE_HPP
#define VG_FRAME_OBJECT_CACHE_HPP

#include <unordered_map>
#include "graphics/global.hpp"

namespace vg
{
    template <typename KeyType, typename ObjectType>
    class FrameObjectCache {
    public:
        FrameObjectCache();
        void begin();
        void end();
        Bool32 hasOldObject(KeyType key);
        ObjectType getOldObject(KeyType key);
    private:
        Bool32 m_isDoing;
        std::unordered_map<KeyType, ObjectType> m_mapObjects;
        std::unordered_map<KeyType, ObjectType> m_mapOldObjects;
    };
} // vg

#include "graphics/util/frame_object_cache.inl"

#endif //VG_FRAME_OBJECT_CACHE_HPP