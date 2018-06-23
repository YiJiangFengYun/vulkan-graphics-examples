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
    protected:
        Bool32 m_isOrthographic;
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
        static const float DEFAULT_FOVY;
        static const float DEFAULT_ASPECT;
        static const float DEFAULT_Z_NEAR;
        static const float DEFAULT_Z_FAR;
        Projector3();
        void updateProj(float fovy, float aspect, float zNear, float zFar);
        float getFovY() const;
        void setFovY(float fovy);
        float getAspect() const;
        void setAspect(float aspect);
        float getZNear() const;
        void setZNear(float zNear);
        float getZFar() const;
        void setZFar(float zFar);
    private:
        float m_fovy, m_aspect, m_zNear, m_zFar;
    };
} // vg


#endif //VG_PROJECTOR_HPP