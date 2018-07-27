#ifndef VG_MATERIAL_DEFAULT_HPP
#define VG_MATERIAL_DEFAULT_HPP

#include "graphics/material/material.hpp"
#include "graphics/pass/pass_default.hpp"

namespace vg
{
    class MaterialPreDepthDefault : public vg::Material
    {
    public:
        MaterialPreDepthDefault();
    private:
    };

    class MaterialLightingDepthDefault : public vg::Material
    {
    public:
        MaterialLightingDepthDefault();
    private:
    };

    class MaterialLightingPointDistDefault : public vg::Material
    {
    public:
        MaterialLightingPointDistDefault();
    private:
        virtual void _beginBind(const BindInfo info, BindResult *pResult) const override;
    };

    extern std::shared_ptr<MaterialPreDepthDefault> pDefaultPreDepthMaterial;
    extern std::shared_ptr<MaterialLightingDepthDefault> pDefaultLightingDepthMaterial;
    extern std::shared_ptr<MaterialLightingPointDistDefault> pDefaultLightingPointDistMaterial;

    extern void createDefaultMaterials();
    extern void destroyDefaultMaterials();
} //vg

#endif //VG_MATERIAL_DEFAULT_HPP