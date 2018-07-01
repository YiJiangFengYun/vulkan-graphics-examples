#ifndef VG_LIGHT_3_H
#define VG_LIGHT_3_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/light.hpp"
#include "graphics/scene/projector.hpp"
#include "graphics/render_target/pre_z_target.hpp"

namespace vg
{
    struct LightDepthRenderInfo
    {
        uint32_t renderCount;
        const BaseProjector *const *pProjectors;
        const PreZTarget *const *pDepthTargets;

        LightDepthRenderInfo(uint32_t renderCount = 0u
            , const BaseProjector *const *pProjectors = nullptr
            , const PreZTarget *const *pDepthTargets = nullptr
            );
    };

    class Light3 : public Light<SpaceType::SPACE_3>
    {
    public:
        Light3();
        virtual LightDepthRenderInfo getDepthRenderInfo() const = 0;
    };

} //namespace kgs

#endif // !VG_LIGHT_3_H
