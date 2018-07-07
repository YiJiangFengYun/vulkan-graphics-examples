#ifndef VG_LIGHT_AMBIENT_3_HPP
#define VG_LIGHT_AMBIENT_3_HPP

#include "graphics/scene/light_3.hpp"

namespace vg
{
    class LightAmbient3 : public Light3<0u, 0u>
    {
    public:
        LightAmbient3();
    protected:
    };
} //vg

#endif //VG_LIGHT_AMBIENT_3_HPP