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
    protected:
        float m_range;
        LightDepthCubeTargets m_cubeTargets;
        std::array<std::shared_ptr<Projector3>, static_cast<size_t>(CubemapFace::RANGE_SIZE)> m_pProjectors;
        std::array<const Projector3 *, static_cast<size_t>(CubemapFace::RANGE_SIZE)> m_refProjectors;

        virtual void _beginRender() override;

        void _syncProjectorTransform();
    };
} //vg

#endif //VG_LIGHT_POINT_3_HPP