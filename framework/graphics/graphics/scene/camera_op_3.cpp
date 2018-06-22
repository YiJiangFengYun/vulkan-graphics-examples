#include "graphics/scene/camera_op_3.hpp"

namespace vg
{
    CameraOP3::CameraOP3()
        : CameraOP<SpaceType::SPACE_3>()
    {
        m_pProjector = std::shared_ptr<BaseProjector>{ new ProjectorOP3()};
    }
} //namespace kgs