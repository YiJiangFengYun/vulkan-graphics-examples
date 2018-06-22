#include "graphics/scene/camera_op_2.hpp"

namespace vg
{
    CameraOP2::CameraOP2()
        : CameraOP<SpaceType::SPACE_2>()
    {
        m_pProjector = std::shared_ptr<BaseProjector>{ new ProjectorOP2()};
    }
} //namespace kgs