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
    const Projector<SPACE_TYPE> *Camera<SPACE_TYPE>::getDirector() const
    {
        Projector<SPACE_TYPE> *pDirector = dynamic_cast<Projector<SPACE_TYPE> *>(m_pProjector.get());
        pDirector->setLocalToWorldMatrix(m_pTransform->getMatrixLocalToWorld());
        return pDirector;
    }

    //template instantiation
    template class Camera<SpaceType::SPACE_2>;
    template class Camera<SpaceType::SPACE_3>;
}