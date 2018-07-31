#include "graphics/light/light_ambient_3.hpp"

namespace vg
{
    LightAmbient3::LightAmbient3()
        : Light3()
        , m_strength(1.0f)
    {
        _setStrength();
        _apply();
    }

    vg::Vector3 LightAmbient3::getStrength() const
    {
        return m_strength;
    }
     
    void LightAmbient3::setStrength(vg::Vector3 value)
    {
        m_strength = value;
        _setStrength();
        _apply();
    }

    LightDepthRenderInfo LightAmbient3::getDepthRenderInfo() const
    {
        LightDepthRenderInfo info = {
            0u,
            nullptr,
            nullptr,
            nullptr,
            0,
        };
        return info;
    }

    void LightAmbient3::_setStrength()
    {
        if (m_data.hasData(VG_LIGHT_AMBIENT3_DATA_STRENGTH_NAME) == VG_FALSE)
        {
            LightDataInfo info = {
                VG_LIGHT_AMBIENT3_DATA_STRENGTH_LAYOUT_PRIORITY,
            };
            m_data.addData(VG_LIGHT_AMBIENT3_DATA_STRENGTH_NAME, info, m_strength);
            m_dataChanged = VG_TRUE;
        } else {
            m_data.setData(VG_LIGHT_AMBIENT3_DATA_STRENGTH_NAME, m_strength);
        }
        m_dataContentChanged = VG_TRUE;
        m_dataContentChanges[VG_LIGHT_AMBIENT3_DATA_STRENGTH_NAME] = VG_TRUE;
    }
} //vg