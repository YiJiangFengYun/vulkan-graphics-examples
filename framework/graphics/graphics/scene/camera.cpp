#include "graphics/scene/camera.hpp"

namespace vg
{
    BaseCamera::BaseCamera()
        : Base(BaseType::SCENE_OBJECT)
        , m_pProjector()
    {
    }

    BaseCamera::~BaseCamera()
    {
        
    }

    Bool32 BaseCamera::getIsOrthographic() const
    {
        return m_pProjector->getIsOrthographic();
    }

    const BaseProjector *BaseCamera::getProjectorBase() const
    {
        return m_pProjector.get();
    }

    template <SpaceType SPACE_TYPE>
    Camera<SPACE_TYPE>::Camera()
        : BaseCamera()
        , Object<SPACE_TYPE>()
    {
        m_objectType = ObjectType::CAMERA;
    }

    template <SpaceType SPACE_TYPE>
    typename Projector<SPACE_TYPE>::MatrixType Camera<SPACE_TYPE>::getProjMatrix() const
    {
        return (dynamic_cast<Projector<SPACE_TYPE> *>(m_pProjector.get()))->getProjMatrix();
    }

    template <SpaceType SPACE_TYPE>
    const BaseProjector *Camera<SPACE_TYPE>::getProjectorBase() const
    {
        Projector<SPACE_TYPE> *pProjector = dynamic_cast<Projector<SPACE_TYPE> *>(m_pProjector.get());
        return pProjector;
    }

    template <SpaceType SPACE_TYPE>
    const Projector<SPACE_TYPE> *Camera<SPACE_TYPE>::getProjector() const
    {
        Projector<SPACE_TYPE> *pProjector = dynamic_cast<Projector<SPACE_TYPE> *>(m_pProjector.get());
        return pProjector;
    }

    template <SpaceType SPACE_TYPE>
    void Camera<SPACE_TYPE>::_addToSpace()
    {
        m_pProjector->setSpace(m_space);
    }

    template <SpaceType SPACE_TYPE>
    void Camera<SPACE_TYPE>::_beginRender()
    {
        Object<SPACE_TYPE>::_beginRender();
        Projector<SPACE_TYPE> *pProjector = dynamic_cast<Projector<SPACE_TYPE> *>(m_pProjector.get());
        pProjector->setLocalToWorldMatrix(m_pTransform->getMatrixLocalToWorld());
    }

    //template instantiation
    template class Camera<SpaceType::SPACE_2>;
    template class Camera<SpaceType::SPACE_3>;
}