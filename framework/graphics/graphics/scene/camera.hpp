#ifndef VG_CAMERA_H
#define VG_CAMERA_H

#include "graphics/scene/object.hpp"
#include "graphics/scene/projector.hpp"

namespace vg
{
    class BaseCamera : public Base
    {
    public:
        BaseCamera();
        virtual ~BaseCamera();
        Bool32 getIsOrthographic() const;
    protected:
        std::shared_ptr<BaseProjector> m_pProjector;
    };

    template <SpaceType SPACE_TYPE>
    class Camera : public BaseCamera, public Object<SPACE_TYPE>
    {
    public:
        Camera();
        typename Projector<SPACE_TYPE>::MatrixType getProjMatrix() const;
        const Projector<SPACE_TYPE> *getDirector() const;
    };
} //namespace kgs

#endif // !VG_CAMERA_H
