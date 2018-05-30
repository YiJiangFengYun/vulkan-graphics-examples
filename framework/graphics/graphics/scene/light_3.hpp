#ifndef VG_LIGHT_3_H
#define VG_LIGHT_3_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/light.hpp"

namespace vg
{
    class Light3 : public Light<SpaceType::SPACE_3>
    {
    public:
        Light3();
    };

} //namespace kgs

#endif // !VG_LIGHT_3_H
