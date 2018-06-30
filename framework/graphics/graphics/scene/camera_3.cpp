#include "graphics/scene/camera_3.hpp"

namespace vg
{
    const float Camera3::DEFAULT_FOV = glm::radians(45.0f);
    const float Camera3::DEFAULT_ASPECT = 1.0f;
    const float Camera3::DEFAULT_DEPTH_NEAR = 0.1f;
    const float Camera3::DEFAULT_DEPTH_FAR = 10.0f;
    Camera3::Camera3()
        : Camera<SpaceType::SPACE_3>()
    {
        m_pProjector = std::shared_ptr<BaseProjector>{ new Projector3()};
    }

    void Camera3::updateProj(float fov, float aspect, float depthNear, float depthFar)
    {
        (dynamic_cast<Projector3 *>(m_pProjector.get()))->updateProj(fov, aspect, depthNear, depthFar);
    }

    float Camera3::getFov() const
    {
        return (dynamic_cast<Projector3 *>(m_pProjector.get()))->getFov();
    }

    void Camera3::setFov(float fov)
    {
        (dynamic_cast<Projector3 *>(m_pProjector.get()))->setFov(fov);
    }

    float Camera3::getAspect() const
    {
        return (dynamic_cast<Projector3 *>(m_pProjector.get()))->getAspect();
    }

    void Camera3::setAspect(float aspect)
    {
        (dynamic_cast<Projector3 *>(m_pProjector.get()))->setAspect(aspect);
    }

    float Camera3::getDepthNear() const
    {
        return (dynamic_cast<Projector3 *>(m_pProjector.get()))->getDepthNear();
    }

    void Camera3::setDepthNear(float depthNear)
    {
        (dynamic_cast<Projector3 *>(m_pProjector.get()))->setDepthNear(depthNear);
    }

    float Camera3::getDepthFar() const
    {
        return (dynamic_cast<Projector3 *>(m_pProjector.get()))->getDepthFar();
    }

    void Camera3::setDepthFar(float depthFar)
    {
        (dynamic_cast<Projector3 *>(m_pProjector.get()))->setDepthFar(depthFar);
    }

} //namespace kgs