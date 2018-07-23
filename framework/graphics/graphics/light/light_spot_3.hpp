#ifndef VG_LIGHT_SPOT_3_HPP
#define VG_LIGHT_SPOT_3_HPP

#include "graphics/scene/light_3.hpp"
#include "graphics/light/light_depth_target_2d.hpp"

namespace vg
{
    #define LIGHT_SPOT3_DATA_RANGE_NAME "light_range"
    //transform of light + projection of light + radius
    #define LIGHT_SPOT3_DATA_SIZE LIGHT_DATA_BASE_SIZE + + static_cast<uint32_t>(sizeof(vg::Matrix4x4)) + static_cast<uint32_t>(sizeof(vg::Vector4))
    #define LIGHT_SPOT3_TEXTURE_COUNT 1u //depth texture.
    class LightSpot3 : public Light3<LIGHT_SPOT3_DATA_SIZE, LIGHT_SPOT3_TEXTURE_COUNT>
    {
    public:
        static const uint32_t DEFAULT_DEPTH_TEXTURE_WIDTH;
        static const uint32_t DEFAULT_DEPTH_TEXTURE_HEIGHT;
        static const float DEFAULT_RANGE;
        static const float DEFAULT_FOV;
        static const vk::Format DEFAULT_FORMAT;
        LightSpot3(float fov = DEFAULT_FOV
            , float range = DEFAULT_RANGE
            , uint32_t depthTextureWidth = DEFAULT_DEPTH_TEXTURE_WIDTH
            , uint32_t depthTextureHeight = DEFAULT_DEPTH_TEXTURE_HEIGHT
            , vk::Format format = DEFAULT_FORMAT
            );
        virtual LightDepthRenderInfo getDepthRenderInfo() const override;
        float getFOV() const;
        void setFOV(float value);
        float getRange() const;
        void setRange(float value);
    protected:
        float m_fov;
        float m_range;
        std::shared_ptr<LightDepthTarget2D> m_pDepthTarget;
        const LightDepthTarget2D *m_refDepthTarget;
        std::shared_ptr<Projector3> m_pProjector;
        const Projector3 *m_refProjector;

        virtual void _beginRender() override;
    };
} //vg

#endif //VG_LIGHT_SPOT_3_HPP