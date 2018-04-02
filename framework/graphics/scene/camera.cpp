#include "graphics/scene/camera.hpp"

namespace vg
{
    BaseCamera::BaseCamera()
        : m_isOrthographic(VG_FALSE)
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