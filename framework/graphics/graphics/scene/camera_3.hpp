#ifndef VG_CAMERA_3_H
#define VG_CAMERA_3_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/camera.hpp"

namespace vg
{
    class Camera3 : public Camera<SpaceType::SPACE_3>
    {
    public:
        static const float DEFAULT_FOV;
        static const float DEFAULT_ASPECT;
        static const float DEFAULT_DEPTH_NEAR;
        static const float DEFAULT_DEPTH_FAR;
        Camera3();
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
    };

} //namespace kgs

#endif // !VG_CAMERA_3_H
