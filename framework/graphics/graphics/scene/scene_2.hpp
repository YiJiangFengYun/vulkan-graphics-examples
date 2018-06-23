#ifndef VG_SCENE_2_H
#define VG_SCENE_2_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/scene.hpp"

namespace vg
{
    class Scene2 : public Scene<SpaceType::SPACE_2>
    {
    public:
        Scene2();
        virtual MatrixType getProjMatrix(const ProjectorType *pProjector) const override;
        virtual BoundsType getProjectionBoundsInWorld(const ProjectorType *pProjector) const override;
        virtual Bool32 isInProjection(const ProjectorType *pProjector
            , const TransformType *pTransform
            , BoundsType bounds
            , fd::Rect2D *projectionRect = nullptr) const override;
        
        virtual Bool32 isInProjection(const ProjectorType *pProjector
            , BoundsType bounds
            , fd::Rect2D *projectionRect = nullptr) const override;
    private:
    };

} //namespace kgs

#endif // !VG_SCENE_2_H
