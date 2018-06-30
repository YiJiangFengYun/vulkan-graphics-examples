#include "graphics/scene/scene_2.hpp"

#include "graphics/util/gemo_util.hpp"
#include "graphics/scene/camera_op_2.hpp"

namespace vg
{
    Scene2::Scene2()
        : Scene<SpaceType::SPACE_2>()
    {

    }

    Scene2::MatrixType Scene2::getProjMatrix(const ProjectorType *pProjector) const
    {
        auto projMatrix = pProjector->getProjMatrix();
        if (m_space.rightHand == VG_FALSE)
        {
            projMatrix[1][1] *= -1;
            return projMatrix;
        }
        else
        {
            return projMatrix;
        }
    }

    Scene2::BoundsType Scene2::getProjectionBoundsInWorld(const ProjectorType *pProjector) const
    {
        using PointType = typename SpaceTypeInfo<SpaceType::SPACE_2>::PointType;
        using MatrixVectorType = typename SpaceTypeInfo<SpaceType::SPACE_2>::MatrixVectorType;
        const ProjectorOP2 *pProjectorOP2 = dynamic_cast<const ProjectorOP2 *>(pProjector);
        auto bounds = pProjectorOP2->getViewBounds();

        //matrix from camera local to world.
        auto matrix = pProjector->getLocalToWorldMatrix();
        auto boundsInWorld = tranBoundsToNewSpace<PointType>(bounds, matrix, VG_FALSE);

        return boundsInWorld;
    }

    Bool32 Scene2::isInProjection(const ProjectorType *pProjector
        , const TransformType *pTransform
        , BoundsType bounds
        , fd::Rect2D *projectionRect) const
    {
        //get MVP matrix.
        auto mvpMatrix = getProjMatrix(pProjector) * pProjector->getWorldToLocalMatrix() * pTransform->getMatrixLocalToWorld();
        auto boundsInProjection = tranBoundsToNewSpace<PointType>(bounds, mvpMatrix, VG_FALSE);
        BoundsType boundsOfProjection(PointType(-1.0f, -1.0f), PointType(1.0f, 1.0f));
        BoundsType intersectionInProjection;
        Bool32 isInsideProjection = VG_FALSE;
        ////check if it is inside projection.
        if (boundsOfProjection.intersects(boundsInProjection, &intersectionInProjection))
        {
            isInsideProjection = VG_TRUE;
            if (projectionRect != nullptr)
            {
                auto min = intersectionInProjection.getMin();
                auto size = intersectionInProjection.getSize();
                projectionRect->x = min.x;
                projectionRect->y = min.y;
                projectionRect->width = size.x;
                projectionRect->height = size.y;
            }
        }

        return isInsideProjection;
    }

    Bool32 Scene2::isInProjection(const ProjectorType *pProjector
        , BoundsType bounds
        , fd::Rect2D *projectionRect) const
    {
        //get VP matrix.
        auto vpMatrix = getProjMatrix(pProjector) * pProjector->getWorldToLocalMatrix();
        auto boundsInProjection = tranBoundsToNewSpace<PointType>(bounds, vpMatrix, VG_FALSE);
        BoundsType boundsOfProjection(PointType(-1.0f, -1.0f), PointType(1.0f, 1.0f));
        BoundsType intersectionInProjection;
        Bool32 isInsideProjection = VG_FALSE;
        ////check if it is inside projection.
        if (boundsOfProjection.intersects(boundsInProjection, &intersectionInProjection))
        {
            isInsideProjection = VG_TRUE;
            if (projectionRect != nullptr)
            {
                auto min = intersectionInProjection.getMin();
                auto size = intersectionInProjection.getSize();
                projectionRect->x = min.x;
                projectionRect->y = min.y;
                projectionRect->width = size.x;
                projectionRect->height = size.y;
            }
        }

        return isInsideProjection;
    }
} //namespace kgs