#include "graphics/scene/camera_3.hpp"

namespace vg
{
    const float Camera3::DEFAULT_FOVY = glm::radians(45.0f);
    const float Camera3::DEFAULT_ASPECT = 1.0f;
    const float Camera3::DEFAULT_Z_NEAR = 0.1f;
    const float Camera3::DEFAULT_Z_FAR = 10.0f;
    Camera3::Camera3()
        : Camera<SpaceType::SPACE_3>()
    {
        m_pProjector = std::shared_ptr<BaseProjector>{ new Projector3()};
    }

    void Camera3::updateProj(float fovy, float aspect, float zNear, float zFar)
    {
        (dynamic_cast<Projector3 *>(m_pProjector.get()))->updateProj(fovy, aspect, zNear, zFar);
    }

    float Camera3::getFovY() const
    {
        return (dynamic_cast<Projector3 *>(m_pProjector.get()))->getFovY();
    }

    void Camera3::setFovY(float fovy)
    {
        (dynamic_cast<Projector3 *>(m_pProjector.get()))->setFovY(fovy);
    }

    float Camera3::getAspect() const
    {
        return (dynamic_cast<Projector3 *>(m_pProjector.get()))->getAspect();
    }

    void Camera3::setAspect(float aspect)
    {
        (dynamic_cast<Projector3 *>(m_pProjector.get()))->setAspect(aspect);
    }

    float Camera3::getZNear() const
    {
        return (dynamic_cast<Projector3 *>(m_pProjector.get()))->getZNear();
    }

    void Camera3::setZNear(float zNear)
    {
        (dynamic_cast<Projector3 *>(m_pProjector.get()))->setZNear(zNear);
    }

    float Camera3::getZFar() const
    {
        return (dynamic_cast<Projector3 *>(m_pProjector.get()))->getZFar();
    }

    void Camera3::setZFar(float zFar)
    {
        (dynamic_cast<Projector3 *>(m_pProjector.get()))->setZFar(zFar);
    }

} //namespace kgs