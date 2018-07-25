#ifndef VG_MATERIAL_DEFAULT_HPP
#define VG_MATERIAL_DEFAULT_HPP

#include "graphics/material/material.hpp"
#include "graphics/pass/pass_default.hpp"

namespace vg
{
    extern std::shared_ptr<Material> pDefaultPreDepthMaterial;
    extern std::shared_ptr<Material> pDefaultLightDepthMaterial;
    extern std::shared_ptr<Material> pDefaultLightPointDistMaterial;

    extern void createDefaultMaterials();
    extern void destroyDefaultMaterials();
} //vg

#endif //VG_MATERIAL_DEFAULT_HPP