#include "graphics/scene/scene_3.hpp"

#include "graphics/util/gemo_util.hpp"
#include "graphics/scene/camera_3.hpp"
#include "graphics/scene/camera_op_3.hpp"

namespace vg
{
    Scene3::Scene3()
        : Scene<SpaceType::SPACE_3>()
    {

    }

    Scene3::MatrixType Scene3::getProjMatrix(const ProjectorType *pProjector) const
    {
        auto projMatrix = pProjector->getProjMatrix();
        if (m_isRightHand == VG_FALSE)
        {
            projMatrix[1][1] *= -1;
        }
        else
        {
            vg::Vector3 eulerAngles = vg::Vector3(glm::radians(-90.0f), 0.0f, 0.0f);
            vg::Quaternion changeToGLCoord = vg::Quaternion(eulerAngles);
            projMatrix = projMatrix * glm::toMat4(changeToGLCoord);
        }
        return projMatrix;
    }

    Scene3::BoundsType Scene3::getProjectionBoundsInWorld(const ProjectorType *pProjector) const
    {
        using PointType = typename SpaceTypeInfo<SpaceType::SPACE_3>::PointType;
        using MatrixVectorType = typename SpaceTypeInfo<SpaceType::SPACE_3>::MatrixVectorType;
        BoundsType bounds;
        if (pProjector->getIsOrthographic() == VG_FALSE)
        {
            const Projector3 *pProjector3 = dynamic_cast<const Projector3 *>(pProjector);
            auto tanHalfFovy = glm::tan(pProjector3->getFovY() / 2.0f);
            float minZ = pProjector3->getZNear();
            float maxZ = pProjector3->getZFar();
            float maxX = maxZ * pProjector3->getAspect() * tanHalfFovy;
            float minX = -maxX;
            float maxY = maxZ * tanHalfFovy;
            float minY = -maxY;
            bounds.setMinMax(PointType(minX, minY, minZ), PointType(maxX, maxY, maxZ));
        }
        else
        {
            const ProjectorOP3 *pProjectorOP3 = dynamic_cast<const ProjectorOP3 *>(pProjector);
            bounds = pProjectorOP3->getViewBounds();
        }

        //matrix from local to world.
        auto matrix = pProjector->getLocalToWorldMatrix();
        auto boundsInWorld = tranBoundsToNewSpace<PointType>(bounds, matrix, VG_FALSE);

        return boundsInWorld;
    }

    Bool32 Scene3::isInProjection(const ProjectorType *pProjector
        , const TransformType *pTransform
        , BoundsType bounds
        , fd::Rect2D *projectionRect
        ) const
    {
        if (pProjector->getIsOrthographic() == VG_TRUE) 
        {
            //get MVP matrix.
            auto mvpMatrix = getProjMatrix(pProjector) * pProjector->getWorldToLocalMatrix() * pTransform->getMatrixLocalToWorld();
            auto boundsInProjection = tranBoundsToNewSpace<PointType>(bounds, mvpMatrix, VG_FALSE);
            BoundsType boundsOfProjection(PointType(-1.0f, -1.0f, 0.0f), PointType(1.0f, 1.0f, 1.0f));
            BoundsType intersectionInProjection;        
            Bool32 isInsideProjection = VG_FALSE;
            ////check if it is inside projection view.
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
        else 
        {
            //avoid error of calculating result projected bounds when bounds cross with z = 0 plane,
            //we should separate to multiply step to caluclate result bounds.
            //1.first, we should get mv matrix to calculate bounds in projectior local space.
            auto mvMatrix = pProjector->getWorldToLocalMatrix() * pTransform->getMatrixLocalToWorld();
            auto boundsInProjectorLocal = tranBoundsToNewSpace<PointType>(bounds, mvMatrix, VG_FALSE);
            //2.then, we clip bounds by z far and z near planes.
            auto min = boundsInProjectorLocal.getMin();
            auto max = boundsInProjectorLocal.getMax();
            const Projector3 *pProjector3 = dynamic_cast<const Projector3 *>(pProjector);
            auto zNear = pProjector3->getZNear();
            auto zFar = pProjector3->getZFar();
            if (min.z < zNear)
            {
                min.z = zNear;
                if (max.z < zNear) max.z = zNear;
            }
            if (max.z > zFar)
            {
                max.z = zFar;
                if (min.z > zFar) min.z = zFar;
            }
            boundsInProjectorLocal.setMinMax(min, max);
            //3. final, we get bounds in normalize device space (projection space).
            auto projMatrx = getProjMatrix(pProjector);
            auto boundsInProjection = tranBoundsToNewSpace<PointType>(boundsInProjectorLocal, projMatrx, VG_TRUE);
            BoundsType boundsOfProjection(PointType(-1.0f, -1.0f, 0.0f), PointType(1.0f, 1.0f, 1.0f));
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
    }

    Bool32 Scene3::isInProjection(const ProjectorType *pProjector
        , BoundsType bounds
        , fd::Rect2D *projectionRect
        ) const
    {
        if (pProjector->getIsOrthographic() == VG_TRUE) 
        {
            //get MVP matrix.
            auto vpMatrix = getProjMatrix(pProjector) * pProjector->getWorldToLocalMatrix();
            auto boundsInProjection = tranBoundsToNewSpace<PointType>(bounds, vpMatrix, VG_FALSE);
            BoundsType boundsOfProjection(PointType(-1.0f, -1.0f, 0.0f), PointType(1.0f, 1.0f, 1.0f));
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
        else 
        {
            //avoid error of calculating result projected bounds when bounds cross with z = 0 plane,
            //we should separate to multiply step to caluclate result bounds.
            //1.first, we should get mv matrix to calculate bounds in projector local space.
            auto vMatrix = pProjector->getWorldToLocalMatrix();
            auto boundsInProjectorLocal = tranBoundsToNewSpace<PointType>(bounds, vMatrix, VG_FALSE);
            //2.then, we clip bounds by z far and z near planes.
            auto min = boundsInProjectorLocal.getMin();
            auto max = boundsInProjectorLocal.getMax();
            const Projector3 *pProjector3 = dynamic_cast<const Projector3 *>(pProjector);
            auto zNear = pProjector3->getZNear();
            auto zFar = pProjector3->getZFar();
            if (min.z < zNear)
            {
                min.z = zNear;
                if (max.z < zNear) max.z = zNear;
            }
            if (max.z > zFar)
            {
                max.z = zFar;
                if (min.z > zFar) min.z = zFar;
            }
            boundsInProjectorLocal.setMinMax(min, max);
            //3. final, we get bounds in normalize device space (projection space).
            auto projMatrx = getProjMatrix(pProjector);
            auto boundsInProjection = tranBoundsToNewSpace<PointType>(boundsInProjectorLocal, projMatrx, VG_TRUE);
            BoundsType boundsOfProjection(PointType(-1.0f, -1.0f, 0.0f), PointType(1.0f, 1.0f, 1.0f));
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
    }


    
} //namespace kgs