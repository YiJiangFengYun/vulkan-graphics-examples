#ifndef VG_LIGHT_POINT_3_HPP
#define VG_LIGHT_POINT_3_HPP

#include "graphics/scene/light_3.hpp"
#include "graphics/light/light_depth_target_cube.hpp"
#include "graphics/scene/projector.hpp"

namespace vg
{
    #define LIGHT_POINT3_DATA_RANGE_NAME "light_range"
    //transform of light + radius
    #define LIGHT_POINT3_DATA_SIZE LIGHT_DATA_BASE_SIZE + static_cast<uint32_t>(sizeof(vg::Vector4))
    #define LIGHT_POINT3_TEXTURE_COUNT 1u //depth texture.

    class LightPoint3 : public Light3<LIGHT_POINT3_DATA_SIZE, LIGHT_POINT3_TEXTURE_COUNT>
    {
    public:
        struct LightDepthRenderData {
            Matrix4x4 cubeFaceTransform[6u];
            Bool32 rightHand;
            float depthNear;
            float depthFar;

            LightDepthRenderData();
        };

        static const uint32_t DEFAULT_DEPTH_TEXTURE_WIDTH;
        static const uint32_t DEFAULT_DEPTH_TEXTURE_HEIGHT;
        static const float DEFAULT_RANGE;

        LightPoint3(float range = DEFAULT_RANGE
            , uint32_t depthTextureWidth = DEFAULT_DEPTH_TEXTURE_WIDTH
            , uint32_t depthTextureHeight = DEFAULT_DEPTH_TEXTURE_HEIGHT
            );
        virtual LightDepthRenderInfo getDepthRenderInfo() const override;
        float getRange() const;
        void setRange(float value);
        const LightDistTargetCube *getLightDistTargetCube() const;
    protected:
        float m_range;
        std::shared_ptr<LightDistTargetCube> m_pDistTarget;
        const LightDistTargetCube *m_refDistTarget;
        std::shared_ptr<Projector3> m_pProjector;
        const Projector3 *m_refProjector;
        LightDepthRenderData m_depthRenderData;
        virtual void _beginRender() override;

        void _resetProjector();
    };
} //vg

#endif //VG_LIGHT_POINT_3_HPP