#ifndef VG_LIGHT_AMBIENT_3_HPP
#define VG_LIGHT_AMBIENT_3_HPP

#include "graphics/scene/light_3.hpp"

namespace vg
{
    #define VG_LIGHT_AMBIENT3_DATA_STRENGTH_NAME "light_strength"
    #define VG_LIGHT_AMBIENT3_DATA_STRENGTH_LAYOUT_PRIORITY VG_LIGHT_DATA_OTHER_MIN_LAYOUT_PRIORITY + 0

    //transform + strength
    #define VG_LIGHT_AMBIENT3_DATA_SIZE LIGHT_DATA_BASE_SIZE + static_cast<uint32_t>(sizeof(vg::Vector4))

    class LightAmbient3 : public Light3<VG_LIGHT_AMBIENT3_DATA_SIZE, 0u>
    {
    public:
        LightAmbient3();
        vg::Vector3 getStrength() const;
        void setStrength(vg::Vector3 value);

        virtual LightDepthRenderInfo getDepthRenderInfo() const override;
    protected:
        vg::Vector3 m_strength;
        void _setStrength();
    };
} //vg

#endif //VG_LIGHT_AMBIENT_3_HPP