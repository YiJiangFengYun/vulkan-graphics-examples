#ifndef VG_LIGHT_AMBIENT_3_HPP
#define VG_LIGHT_AMBIENT_3_HPP

#include "graphics/scene/light_3.hpp"

namespace vg
{
    extern const LightRegisterInfo LIGHT_AMBIENT3_REGISTER_INFO;
    class LightAmbient3 : public Light3<LIGHT_AMBIENT3_REGISTER_INFO>
    {
    public:
        LightAmbient3();
    protected:
    };
} //vg

#endif //VG_LIGHT_AMBIENT_3_HPP