#ifndef VG_PASS_DEFAULT_HPP
#define VG_PASS_DEFAULT_HPP

#include "graphics/global.hpp"
#include "graphics/pass/pass.hpp"
namespace vg
{
    extern std::shared_ptr<Shader> pDefaultPreDepthShader;
    extern std::shared_ptr<Pass> pDefaultPreDepthPass;
    extern std::shared_ptr<Shader> pDefaultLightingDepthShader;
    extern std::shared_ptr<Pass> pDefaultLightingDepthPass;
    extern std::shared_ptr<Shader> pDefaultLightingPointDistShader;
    extern std::shared_ptr<Pass> pDefaultLightingPointDistPass;

    extern void createDefaultPasses();
    extern void destroyDefaultPasses();
} //vg

#endif //VG_PASS_DEFAULT_HPP