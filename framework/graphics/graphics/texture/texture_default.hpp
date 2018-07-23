#ifndef VG_TEXTURE_DEFAULT_HPP
#define VG_TEXTURE_DEFAULT_HPP

#include "graphics/global.hpp"
#include "graphics/texture/texture_1d.hpp"
#include "graphics/texture/texture_2d.hpp"
#include "graphics/texture/texture_3d.hpp"
#include "graphics/texture/texture_cube.hpp"
#include "graphics/texture/texture_1d_array.hpp"
#include "graphics/texture/texture_2d_array.hpp"
#include "graphics/texture/texture_cube_array.hpp"

namespace vg
{
    extern std::shared_ptr<Texture1D> pDefaultTexture1D;
    extern std::shared_ptr<Texture2D> pDefaultTexture2D;
    extern std::shared_ptr<Texture3D> pDefaultTexture3D;
    extern std::shared_ptr<TextureCube> pDefaultTextureCube;
    extern std::shared_ptr<Texture1DArray> pDefaultTexture1DArray;
    extern std::shared_ptr<Texture2DArray> pDefaultTexture2DArray;
    extern std::shared_ptr<TextureCubeArray> pDefaultTextureCubeArray;

    extern std::array<std::pair<vk::ImageViewType, const Texture *>, 7u> arrImageViewTypeToDefaultTexture;

    extern const Texture *getDefaultTexture(vk::ImageViewType type);

    extern void createDefaultTextures();
    extern void destroyDefaultTextures();
} //vg

#endif //VG_TEXTURE_DEFAULT_HPP