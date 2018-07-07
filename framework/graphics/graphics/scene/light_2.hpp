#ifndef VG_LIGHT_2_H
#define VG_LIGHT_2_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/light.hpp"

namespace vg
{
    template <const LightRegisterInfo &registerInfo>
    class Light2 : public Light<SpaceType::SPACE_2, registerInfo>
    {
    public:
        Light2();
    };

} //namespace kgs

#include "graphics/scene/light_2.inl"
#endif // !VG_LIGHT_2_H
