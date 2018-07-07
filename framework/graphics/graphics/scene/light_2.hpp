#ifndef VG_LIGHT_2_H
#define VG_LIGHT_2_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/light.hpp"

namespace vg
{
    template <uint32_t DATA_SIZE, uint32_t TEXTURE_COUNT>
    class Light2 : public Light<SpaceType::SPACE_2, DATA_SIZE, TEXTURE_COUNT>
    {
    public:
        Light2();
    };

} //namespace kgs

#include "graphics/scene/light_2.inl"
#endif // !VG_LIGHT_2_H
