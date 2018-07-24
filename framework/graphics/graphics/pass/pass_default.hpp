#ifndef VG_PASS_DEFAULT_HPP
#define VG_PASS_DEFAULT_HPP

#include "graphics/global.hpp"
#include "graphics/pass/pass.hpp"
namespace vg
{
    extern std::shared_ptr<Shader> pDefaultPreDepthShader;
    extern std::shared_ptr<Pass> pDefaultPreDepthPass;
    extern std::shared_ptr<Shader> pDefaultLightDepthShader;
    extern std::shared_ptr<Pass> pDefaultLightDepthPass;
    extern std::shared_ptr<Shader> pDefaultLightPointDistShader;
    extern std::shared_ptr<Pass> pDefaultLightPointDistPass;

    extern void createDefaultPasses();
    extern void destroyDefaultPasses();
} //vg

#endif //VG_PASS_DEFAULT_HPP