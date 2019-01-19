#ifndef VG_VISUAL_OBJECT_2_H
#define VG_VISUAL_OBJECT_2_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/visual_object.hpp"

namespace vg
{
    class VisualObject2 : public VisualObject<SpaceType::SPACE_2>
    {
    public:
        VisualObject2();

        template <class VisualizerType>
        VisualObject2()
            : VisualObject<SpaceType::SPACE_2><VisualizerType>()
        {

        }

        Bool32 getHasClipRect() const;
        void setHasClipRect(Bool32 value);
        uint32_t getClipRectCount() const;
        const fd::Rect2D *getClipRects() const;
        void updateClipRects(fd::ArrayProxy<fd::Rect2D> rects, uint32_t offset = 0u);
        void updateClipRects(fd::Rect2D rect, uint32_t count, uint32_t offset = 0u);

        virtual Matrix4x4 _getModelMatrix() const override;

        virtual void setMesh(MeshDimType *pMesh) override;

        virtual void setMesh(MeshDimType *pMesh
            , uint32_t subMeshOffset
            , uint32_t subMeshCount) override;

        virtual void updateSubMeshInfo(uint32_t subMeshOffset, uint32_t subMeshCount) override;
    private:
        Bool32 m_hasClipRect;
        //Valid range of ClipRect is [(0, 0), (1, 1)]
        std::vector<fd::Rect2D> m_clipRects;
    };

} //namespace kgs

#endif // !VG_VISUAL_OBJECT_2_H
