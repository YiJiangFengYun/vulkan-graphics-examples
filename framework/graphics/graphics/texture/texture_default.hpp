#ifndef VG_TEXTURE_DEFAULT_HPP
#define VG_TEXTURE_DEFAULT_HPP

#include "graphics/global.hpp"
#include "graphics/texture/texture_2d.hpp"

namespace vg
{
    extern std::shared_ptr<Texture2D> pDefaultTexture2D;
    extern void createDefaultTexture();
    extern void destroyDefaultTexture();
} //vg

#endif //VG_TEXTURE_DEFAULT_HPP