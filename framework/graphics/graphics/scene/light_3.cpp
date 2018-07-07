#include "graphics/scene/light_3.hpp"

namespace vg
{
    LightDepthRenderInfo::LightDepthRenderInfo(uint32_t renderCount
        , const BaseProjector *const *pProjectors
        , const PreDepthTarget *const *pDepthTargets
        )
        : renderCount(renderCount)
        , pProjectors(pProjectors)
        , pDepthTargets(pDepthTargets)
    {

    }
} //namespace kgs