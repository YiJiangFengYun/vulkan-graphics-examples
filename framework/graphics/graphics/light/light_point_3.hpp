#ifndef VG_LIGHT_POINT_3_HPP
#define VG_LIGHT_POINT_3_HPP

#include "graphics/scene/light_3.hpp"
#include "graphics/light/light_depth_target_cube.hpp"
#include "graphics/scene/projector.hpp"

namespace vg
{
    extern const LightRegisterInfo LIGHT_POINT3_REGISTER_INFO;
    class LightPoint3 : public Light3<LIGHT_POINT3_REGISTER_INFO>
    {
    public:
        static const uint32_t DEFAULT_DEPTH_TEXTURE_WIDTH;
        static const uint32_t DEFAULT_DEPTH_TEXTURE_HEIGHT;
        static const float DEFAULT_RADIUS;

        LightPoint3(float radius = DEFAULT_RADIUS
            , uint32_t depthTextureWidth = DEFAULT_DEPTH_TEXTURE_WIDTH
            , uint32_t depthTextureHeight = DEFAULT_DEPTH_TEXTURE_HEIGHT
            );
        virtual LightDepthRenderInfo getDepthRenderInfo() const override;
    protected:
        float m_radius;
        LightDepthCubeTargets m_cubeTargets;
        std::array<std::shared_ptr<Projector3>, static_cast<size_t>(CubemapFace::RANGE_SIZE)> m_pProjectors;
        std::array<const Projector3 *, static_cast<size_t>(CubemapFace::RANGE_SIZE)> m_refProjectors;
    };
} //vg

#endif //VG_LIGHT_POINT_3_HPP