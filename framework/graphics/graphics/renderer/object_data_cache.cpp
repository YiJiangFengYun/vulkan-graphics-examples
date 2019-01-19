#include "graphics/renderer/object_data_cache.hpp"

namespace vg
{
    RendererObjectData::RendererObjectData(Bool32 hasClipRect
        , std::vector<fd::Rect2D> clipRects
    )
        : hasClipRect(hasClipRect)
        , clipRects(clipRects)
    {

    }

    Bool32 RendererObjectData::getHasClipRect() const
    {
        return hasClipRect;
    }

    void RendererObjectData::setHasClipRect(Bool32 value)
    {
        hasClipRect = value;
    }

    uint32_t RendererObjectData::getClipRectCount() const
    {
        return static_cast<uint32_t>(clipRects.size());
    }
        
    const fd::Rect2D *RendererObjectData::getClipRects() const
    {
        return clipRects.data();
    }

    void RendererObjectData::updateClipRects(fd::ArrayProxy<fd::Rect2D> rects, uint32_t offset)
    {
        uint32_t count = static_cast<uint32_t>(rects.size());
        if (clipRects.size() < offset + count) clipRects.resize(offset + count);
        for (uint32_t i = 0; i < count; ++i)
        {
            clipRects[offset] = *(rects.data() + i);
            ++offset;
        }
    }

    void RendererObjectData::updateClipRects(const fd::Rect2D *pRects, uint32_t count, uint32_t offset )
    {
        if (clipRects.size() < offset + count) clipRects.resize(offset + count);
        for (uint32_t i = 0; i < count; ++i)
        {
            clipRects[offset] = *(pRects + i);
            ++offset;
        }
    }
        
    void RendererObjectData::updateClipRects(fd::Rect2D rect, uint32_t count, uint32_t offset)
    {
        if (clipRects.size() < offset + count) clipRects.resize(offset + count);
        for (uint32_t i = 0; i < count; ++i)
        {
            clipRects[offset] = rect;
            ++offset;
        }
    }

    RendererObjectDataCache::RendererObjectDataCache()
        : m_mapDatas()
        , m_mapDatasBack()
    {

    }

    RendererObjectDataCache::~RendererObjectDataCache()
    {

    }

    void RendererObjectDataCache::begin()
    {
        m_mapDatasBack = m_mapDatas;
        m_mapDatas.clear();
    }

    RendererObjectData *RendererObjectDataCache::get(InstanceID objectID)
    {
        RendererObjectData * pCurr;
        auto key = objectID;
        if (m_mapDatas[key] != nullptr) {
            pCurr = m_mapDatas[key].get();
        } else if (m_mapDatasBack[key] != nullptr) {
            m_mapDatas[key] = m_mapDatasBack[key];
            m_mapDatasBack.erase(key);
            pCurr = m_mapDatas[key].get();
        } else {
            auto pNew = std::shared_ptr<RendererObjectData>{new RendererObjectData()};
            m_mapDatas[key] = pNew;
            pCurr = pNew.get();
        }
        return pCurr;
    }

    void RendererObjectDataCache::end()
    {
        //Delete useless renderer passes.
        m_mapDatasBack.clear();
    }

}