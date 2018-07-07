#ifndef VG_LIGHT_DIRECT_3_HPP
#define VG_LIGHT_DIRECT_3_HPP

#include "graphics/scene/light_3.hpp"

namespace vg
{
    extern const LightRegisterInfo LIGHT_DIRECT3_REGISTER_INFO;
    class LightDirect3 : public Light3<LIGHT_DIRECT3_REGISTER_INFO>
    {
    public:
        LightDirect3();
    protected:
    };
} //vg

#endif //VG_LIGHT_DIRECT_3_HPP