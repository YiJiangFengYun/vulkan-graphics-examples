#include "graphics/scene/camera_op.hpp"

namespace vg
{
    template <SpaceType SPACE_TYPE>
    CameraOP<SPACE_TYPE>::CameraOP()
        : Camera<SPACE_TYPE>()
    {
    }

    template <SpaceType SPACE_TYPE>
    void CameraOP<SPACE_TYPE>::updateProj(BoundsType viewBounds)
    {
        (dynamic_cast<ProjectorOP<SPACE_TYPE> *>(m_pProjector.get()))->updateProj(viewBounds);
    }

    template <SpaceType SPACE_TYPE>
    typename CameraOP<SPACE_TYPE>::BoundsType CameraOP<SPACE_TYPE>::getViewBounds() const
    {
        return (dynamic_cast<ProjectorOP<SPACE_TYPE> *>(m_pProjector.get()))->getViewBounds();
    }

    //template instantiation
    template class CameraOP<SpaceType::SPACE_2>;
    template class CameraOP<SpaceType::SPACE_3>;
} //namespace kgs