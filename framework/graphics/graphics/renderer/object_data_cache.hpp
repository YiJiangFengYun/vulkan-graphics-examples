#include "graphics/scene/visual_object.hpp"

namespace vg
{
    struct RendererObjectData {
        Bool32 hasClipRect;
        //Valid range of ClipRect is [(0, 0), (1, 1)]
        std::vector<fd::Rect2D> clipRects;

        RendererObjectData(Bool32 hasClipRect = VG_FALSE
            , std::vector<fd::Rect2D> clipRects = std::vector<fd::Rect2D>()
        );

        Bool32 getHasClipRect() const;
        void setHasClipRect(Bool32 value);
        uint32_t getClipRectCount() const;
        const fd::Rect2D *getClipRects() const;
        void updateClipRects(fd::ArrayProxy<fd::Rect2D> rects, uint32_t offset = 0u);
        void updateClipRects(fd::Rect2D rect, uint32_t count, uint32_t offset = 0u);

    };

    class RendererObjectDataCache
    {
    public:
        RendererObjectDataCache();
        ~RendererObjectDataCache();
        void begin();
        RendererObjectData *get(InstanceID objectID);
        void end();

    private:
        //Map between pass and renderer pass, key is instance ID of pass.
        std::unordered_map<InstanceID, std::shared_ptr<RendererObjectData>> m_mapDatas;
        std::unordered_map<InstanceID, std::shared_ptr<RendererObjectData>> m_mapDatasBack;
    };
}