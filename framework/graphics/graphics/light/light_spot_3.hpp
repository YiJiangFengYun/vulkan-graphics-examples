#ifndef VG_LIGHT_SPOT_3_HPP
#define VG_LIGHT_SPOT_3_HPP

#include "graphics/scene/light_3.hpp"
#include "graphics/light/light_depth_target_2d.hpp"

namespace vg
{
    #define LIGHT_SPOT3_DATA_RADIUS_NAME "light_radius"
    //transform of light + projection of light + radius
    #define LIGHT_SPOT3_DATA_SIZE LIGHT_DATA_BASE_SIZE + + static_cast<uint32_t>(sizeof(vg::Matrix4x4)) + static_cast<uint32_t>(sizeof(vg::Vector4))
    #define LIGHT_SPOT3_TEXTURE_COUNT 1u //depth texture.
    class LightSpot3 : public Light3<LIGHT_SPOT3_DATA_SIZE, LIGHT_SPOT3_TEXTURE_COUNT>
    {
    public:
        static const uint32_t DEFAULT_DEPTH_TEXTURE_WIDTH;
        static const uint32_t DEFAULT_DEPTH_TEXTURE_HEIGHT;
        static const float DEFAULT_RADIUS;
        static const float DEFAULT_FOV;
        LightSpot3(float fov = DEFAULT_FOV
            , float radius = DEFAULT_RADIUS
            , uint32_t depthTextureWidth = DEFAULT_DEPTH_TEXTURE_WIDTH
            , uint32_t depthTextureHeight = DEFAULT_DEPTH_TEXTURE_HEIGHT
            );
        virtual LightDepthRenderInfo getDepthRenderInfo() const override;
    protected:
        float m_fov;
        float m_radius;
        std::shared_ptr<LightDepthTarget2D> m_pDepthTarget;
        const LightDepthTarget2D *m_refDepthTarget;
        std::shared_ptr<Projector3> m_pProjector;
        const Projector3 *m_refProjector;

        virtual void _beginRender();
    };
} //vg

#endif //VG_LIGHT_SPOT_3_HPP