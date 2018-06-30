#ifndef VG_PROJECTOR_HPP
#define VG_PROJECTOR_HPP

#include "graphics/global.hpp"
#include "graphics/scene/space_info.hpp"

namespace vg
{
    class BaseProjector : public Base
    {
    public:
        BaseProjector();
        virtual ~BaseProjector();
        Bool32 getIsOrthographic() const;
        const Space &getSpace() const;
        void setSpace(const Space &space);
    protected:
        Bool32 m_isOrthographic;
        Space m_space;
    };

    template <SpaceType SPACE_TYPE>
    class Projector : public BaseProjector
    {
    public:
        using MatrixType = typename SpaceTypeInfo<SPACE_TYPE>::MatrixType;
        Projector();
        MatrixType getProjMatrix() const;
        MatrixType getTransformMatrix() const;
        MatrixType getLocalToWorldMatrix() const;
        MatrixType getWorldToLocalMatrix() const;
        void setTransformMatrix(const MatrixType &matrix);
        void setLocalToWorldMatrix(const MatrixType &matrix);
    protected:
        MatrixType m_projMatrix;
        MatrixType m_localToWorldMatrix;
        MatrixType m_worldToLocalMatrix;
    };

    template <SpaceType SPACE_TYPE>
    class ProjectorOP : public Projector<SPACE_TYPE>
    {
    public:
        using BoundsType = fd::Bounds<typename SpaceTypeInfo<SPACE_TYPE>::PointType>;        
        ProjectorOP();
        void updateProj(BoundsType viewBounds);
        BoundsType getViewBounds() const;
    protected:
        BoundsType m_viewBounds;
        virtual void _applyProj() = 0;
    private:
        
    };

    class ProjectorOP2 : public ProjectorOP<SpaceType::SPACE_2>
    {
    public:
        ProjectorOP2();
        virtual void _applyProj() override;
    private:

    };

    class ProjectorOP3 : public ProjectorOP<SpaceType::SPACE_3>
    {
    public:
        ProjectorOP3();
    private:
        virtual void _applyProj() override;
    };

    class Projector3 : public Projector<SpaceType::SPACE_3>
    {
    public:
        static const float DEFAULT_FOV;
        static const float DEFAULT_ASPECT;
        static const float DEFAULT_DEPTH_NEAR;
        static const float DEFAULT_DEPTH_FAR;
        Projector3();
        void updateProj(float fov, float aspect, float depthNear, float depthFar);
        float getFov() const;
        void setFov(float fov);
        float getAspect() const;
        void setAspect(float aspect);
        float getDepthNear() const;
        void setDepthNear(float depthNear);
        float getDepthFar() const;
        void setDepthFar(float depthFar);
    private:
        float m_fov, m_aspect, m_depthNear, m_depthFar;
    };
} // vg


#endif //VG_PROJECTOR_HPP