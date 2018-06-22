#ifndef VG_CAMERA_3_H
#define VG_CAMERA_3_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/camera.hpp"

namespace vg
{
    class Camera3 : public Camera<SpaceType::SPACE_3>
    {
    public:
        static const float DEFAULT_FOVY;
        static const float DEFAULT_ASPECT;
        static const float DEFAULT_Z_NEAR;
        static const float DEFAULT_Z_FAR;
        Camera3();
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
    };

} //namespace kgs

#endif // !VG_CAMERA_3_H
