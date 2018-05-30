#ifndef VG_SCENE_3_H
#define VG_SCENE_3_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/scene.hpp"

namespace vg
{
    class Scene3 : public Scene<SpaceType::SPACE_3>
    {
    public:
        Scene3();
        virtual MatrixType getProjMatrix(const CameraType *pCamera) const override;
        virtual BoundsType getViewBoundsInWorld(const CameraType *pCamera) const override;
        virtual Bool32 isInView(const CameraType *pCamera
            , const TransformType *pTransform
            , BoundsType bounds
            , fd::Rect2D *viewRect = nullptr) const override;
        virtual Bool32 isInView(const CameraType *pCamera
            , BoundsType bounds
            , fd::Rect2D *viewRect = nullptr) const override;
    };

} //namespace kgs

#endif // !VG_SCENE_3_H
