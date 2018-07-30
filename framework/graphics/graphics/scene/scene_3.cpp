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
        return m_space.getVulkanProjMatrix(projMatrix);
    }

    Scene3::BoundsType Scene3::getProjectionBoundsInWorld(const ProjectorType *pProjector) const
    {
        using PointType = typename SpaceTypeInfo<SpaceType::SPACE_3>::PointType;
        using MatrixVectorType = typename SpaceTypeInfo<SpaceType::SPACE_3>::MatrixVectorType;
        BoundsType bounds;
        if (pProjector->getOrthographic() == VG_FALSE)
        {
            const Projector3 *pProjector3 = dynamic_cast<const Projector3 *>(pProjector);
            auto tanHalfFovy = glm::tan(pProjector3->getFov() / 2.0f);
            if (pProjector3->getSpace().rightHand == VG_TRUE)
            {
                float minY = pProjector3->getDepthNear();
                float maxY = pProjector3->getDepthFar();
                float maxX = maxY * pProjector3->getAspect() * tanHalfFovy;
                float minX = -maxX;
                float maxZ = maxY * tanHalfFovy;
                float minZ = -maxZ;
                bounds.setMinMax(PointType(minX, minY, minZ), PointType(maxX, maxY, maxZ));
            }
            else
            {
                float minZ = pProjector3->getDepthNear();
                float maxZ = pProjector3->getDepthFar();
                float maxX = maxZ * pProjector3->getAspect() * tanHalfFovy;
                float minX = -maxX;
                float maxY = maxZ * tanHalfFovy;
                float minY = -maxY;
                bounds.setMinMax(PointType(minX, minY, minZ), PointType(maxX, maxY, maxZ));
            }
            
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
        if (pProjector->getOmniDirectional() == VG_FALSE)
        {
            if (pProjector->getOrthographic() == VG_TRUE) 
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
                if (pProjector3->getSpace().rightHand == VG_TRUE)
                {
                    auto yNear = pProjector3->getDepthNear();
                    auto yFar = pProjector3->getDepthFar();
                    if (min.y < yNear)
                    {
                        min.y = yNear;
                        if (max.y < yNear) max.y = yNear;
                    }
                    if (max.y > yFar)
                    {
                        max.y = yFar;
                        if (min.y > yFar) min.y = yFar;
                    }
                }
                else
                {
                    auto zNear = pProjector3->getDepthNear();
                    auto zFar = pProjector3->getDepthFar();
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
        else
        {
            BoundsType boundsOfProjection;
            if (pProjector->getOrthographic() == VG_TRUE)
            {
                const ProjectorOP3 *pProjector3 = dynamic_cast<const ProjectorOP3 *>(pProjector);
                auto viewBounds = pProjector3->getViewBounds();
                auto min = viewBounds.getMin();
                auto max = viewBounds.getMax();
                if (pProjector->getSpace().rightHand == VG_TRUE)
                {
                    boundsOfProjection.setMinMax(Vector3(min.x, -max.y, min.z),
                        Vector3(max.x, max.y, max.z));
                }
                else
                {
                    boundsOfProjection.setMinMax(Vector3(min.x, min.y, - max.z),
                        Vector3(max.x, max.y, max.z));
                }
            }
            else
            {
                const Projector3 *pProjector3 = dynamic_cast<const Projector3 *>(pProjector);
                float depthFar = pProjector3->getDepthFar();
                if (pProjector->getSpace().rightHand == VG_TRUE)
                {
                    float zMax = std::tanf(pProjector3->getFov() / 2) * depthFar;
                    float xMax = zMax * pProjector3->getAspect();
                    boundsOfProjection.setMinMax(Vector3(-xMax, -depthFar, -zMax), Vector3(xMax, depthFar, zMax));
                }
                else
                {
                    float yMax = std::tanf(pProjector3->getFov() / 2) * depthFar;
                    float xMax = yMax * pProjector3->getAspect();
                    boundsOfProjection.setMinMax(Vector3(-xMax, -yMax, -depthFar), Vector3(xMax, yMax, depthFar));
                }
            }
            auto mvMatrix = pProjector->getWorldToLocalMatrix() * pTransform->getMatrixLocalToWorld();
            auto boundsInProjectorLocal = tranBoundsToNewSpace<PointType>(bounds, mvMatrix, VG_FALSE);
            Bool32 isInsideProjection = VG_FALSE;
            if (boundsOfProjection.intersects(boundsInProjectorLocal, nullptr)) {
                isInsideProjection = VG_TRUE;
                if (projectionRect != nullptr)
                {
                    projectionRect->x = -1.0f;
                    projectionRect->y = -1.0f;
                    projectionRect->width = 2.0f;
                    projectionRect->height = 2.0f;
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
        if (pProjector->getOmniDirectional() == VG_FALSE)
        {
            if (pProjector->getOrthographic() == VG_TRUE) 
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
                //2.then, we clip bounds by depth far and depth near planes.
                auto min = boundsInProjectorLocal.getMin();
                auto max = boundsInProjectorLocal.getMax();
                const Projector3 *pProjector3 = dynamic_cast<const Projector3 *>(pProjector);
                if (pProjector3->getSpace().rightHand == VG_TRUE)
                {
                    auto yNear = pProjector3->getDepthNear();
                    auto yFar = pProjector3->getDepthFar();
                    if (min.y < yNear)
                    {
                        min.y = yNear;
                        if (max.y < yNear) max.y = yNear;
                    }
                    if (max.y > yFar)
                    {
                        max.y = yFar;
                        if (min.y > yFar) min.y = yFar;
                    }
                } 
                else
                {
                    auto zNear = pProjector3->getDepthNear();
                    auto zFar = pProjector3->getDepthFar();
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
        else
        {
            BoundsType boundsOfProjection;
            if (pProjector->getOrthographic() == VG_TRUE)
            {
                const ProjectorOP3 *pProjector3 = dynamic_cast<const ProjectorOP3 *>(pProjector);
                auto viewBounds = pProjector3->getViewBounds();
                auto min = viewBounds.getMin();
                auto max = viewBounds.getMax();
                if (pProjector->getSpace().rightHand == VG_TRUE)
                {
                    boundsOfProjection.setMinMax(Vector3(min.x, -max.y, min.z),
                        Vector3(max.x, max.y, max.z));
                }
                else
                {
                    boundsOfProjection.setMinMax(Vector3(min.x, min.y, - max.z),
                        Vector3(max.x, max.y, max.z));
                }
            }
            else
            {
                const Projector3 *pProjector3 = dynamic_cast<const Projector3 *>(pProjector);
                float depthFar = pProjector3->getDepthFar();
                if (pProjector->getSpace().rightHand == VG_TRUE)
                {
                    float zMax = std::tanf(pProjector3->getFov() / 2) * depthFar;
                    float xMax = zMax * pProjector3->getAspect();
                    boundsOfProjection.setMinMax(Vector3(-xMax, -depthFar, -zMax), Vector3(xMax, depthFar, zMax));
                }
                else
                {
                    float yMax = std::tanf(pProjector3->getFov() / 2) * depthFar;
                    float xMax = yMax * pProjector3->getAspect();
                    boundsOfProjection.setMinMax(Vector3(-xMax, -yMax, -depthFar), Vector3(xMax, yMax, depthFar));
                }
            }
            auto vMatrix = pProjector->getWorldToLocalMatrix();
            auto boundsInProjectorLocal = tranBoundsToNewSpace<PointType>(bounds, vMatrix, VG_FALSE);
            Bool32 isInsideProjection = VG_FALSE;
            if (boundsOfProjection.intersects(boundsInProjectorLocal, nullptr)) {
                isInsideProjection = VG_TRUE;
                if (projectionRect != nullptr)
                {
                    projectionRect->x = -1.0f;
                    projectionRect->y = -1.0f;
                    projectionRect->width = 2.0f;
                    projectionRect->height = 2.0f;
                }
            }
            return isInsideProjection;
        }
    }


    
} //namespace kgs