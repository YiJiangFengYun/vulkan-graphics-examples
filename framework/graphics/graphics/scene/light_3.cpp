#include "graphics/scene/light_3.hpp"

namespace vg
{
    LightDepthRenderInfo::LightDepthRenderInfo(uint32_t renderCount
        , const BaseProjector *const *pProjectors
        , const PreZTarget *const *pDepthTargets
        )
        : renderCount(renderCount)
        , pProjectors(pProjectors)
        , pDepthTargets(pDepthTargets)
    {

    }

    Light3::Light3()
        : Light<SpaceType::SPACE_3>()
    {

    }
} //namespace kgs