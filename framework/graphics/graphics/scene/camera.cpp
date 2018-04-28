#include "graphics/scene/camera.hpp"

namespace vg
{
    BaseCamera::BaseCamera()
        : Base(BaseType::SCENE_OBJECT)
        , m_isOrthographic(VG_FALSE)
    {

    }

    BaseCamera::~BaseCamera()
    {
        
    }

    Bool32 BaseCamera::getIsOrthographic() const
    {
        return m_isOrthographic;
    }
}