#ifndef VG_LIGHT_DIRECT_3_HPP
#define VG_LIGHT_DIRECT_3_HPP

#include "graphics/scene/light_3.hpp"
#include "graphics/light/light_depth_target_2d.hpp"

namespace vg
{
    #define VG_LIGHT_DIRECT3_DATA_RANGE_NAME "light_range"
    #define VG_LIGHT_DIRECT3_DATA_RANGE_LAYOUT_PRIORITY VG_LIGHT_DATA_OTHER_MIN_LAYOUT_PRIORITY + 0
    #define VG_LIGHT_DIRECT3_DATA_STRENGTH_NAME "light_strength"
    #define VG_LIGHT_DIRECT3_DATA_STRENGTH_LAYOUT_PRIORITY VG_LIGHT_DATA_OTHER_MIN_LAYOUT_PRIORITY + 1
    //transform of light + projection of light + range + strength
    #define VG_LIGHT_DIRECT3_DATA_SIZE LIGHT_DATA_BASE_SIZE + static_cast<uint32_t>(sizeof(vg::Matrix4x4)) + static_cast<uint32_t>(sizeof(vg::Vector4)) + \
        static_cast<uint32_t>(sizeof(vg::Vector4))
    #define VG_LIGHT_SPOT3_TEXTURE_COUNT 1u //depth texture.
    class LightDirect3 : public Light3<VG_LIGHT_DIRECT3_DATA_SIZE, VG_LIGHT_SPOT3_TEXTURE_COUNT>
    {
    public:
        static const uint32_t DEFAULT_DEPTH_TEXTURE_WIDTH;
        static const uint32_t DEFAULT_DEPTH_TEXTURE_HEIGHT;
        static const vk::Format DEFAULT_FORMAT;
        static const float DEFAULT_WIDTH;
        static const float DEFAULT_HEIGHT;
        static const float DEFAULT_RANGE;

        LightDirect3(float width = DEFAULT_WIDTH
            , float height = DEFAULT_HEIGHT
            , float range = DEFAULT_RANGE
            , uint32_t depthTextureWidth = DEFAULT_DEPTH_TEXTURE_WIDTH
            , uint32_t depthTextureHeight = DEFAULT_DEPTH_TEXTURE_HEIGHT
            , vk::Format format = DEFAULT_FORMAT
            );
        virtual LightDepthRenderInfo getDepthRenderInfo() const override;
        float getWidth() const;
        void setWidth(float value);
        float getHeight() const;
        void setHeight(float value);
        float getRange() const;
        void setRange(float value);
        vg::Vector3 getStrength() const;
        void setStrength(vg::Vector3 value);
    protected:
        float m_width;
        float m_height;
        float m_range;
        vg::Vector3 m_strength;
        std::shared_ptr<LightDepthTarget2D> m_pDepthTarget;
        const LightDepthTarget2D *m_refDepthTarget;
        std::shared_ptr<ProjectorOP3> m_pProjector;
        const ProjectorOP3 *m_refProjector;

        virtual void _beginRender() override;
        void _setProjector();
        void _setRange();
        void _setProjection();
        void _setStrength();
        void _setDepthTexture();
    };
} //vg

#endif //VG_LIGHT_DIRECT_3_HPP