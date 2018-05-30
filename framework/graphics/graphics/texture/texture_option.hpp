#ifndef VG_TEXTURE_TYPE_H
#define VG_TEXTURE_TYPE_H

#include <utility>
#include <Array>
#include <map>
#include <vulkan/vulkan.hpp>

namespace vg
{
    enum class TextureType
    {
        TEX_1D,
        TEX_1D_ARRAY,
        TEX_2D,
        TEX_2D_ARRAY,
        TEX_3D,
        CUBE,
        CUBE_ARRARY,
        COLOR_ATTACHMENT,
        DEPTH_STENCIL_ATTACHMENT,
        TEX_2D_COLOR_ATTACHMENT,
        TEX_2D_DEPTH_STENCIL_ATTACHMENT,
        TEX_2D_DEPTH_ATTACHMENT,
        BEGIN_RANGE = TEX_1D,
        END_RANGE = TEX_2D_DEPTH_ATTACHMENT,
        RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
    };

    enum class CubemapFace
    {
        POSITIVE_X,
        NEGATIVE_X,
        POSITIVE_Y,
        NEGATIVE_Y,
        POSITIVE_Z,
        NEGATIVE_Z,
        BEGIN_RANGE = POSITIVE_X,
        END_RANGE = NEGATIVE_Z,
        RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
    };

    extern std::map<TextureType, std::string> mapTextureTypeToName;
    extern std::array<std::pair<TextureType, vk::ImageType>, static_cast<size_t>(TextureType::RANGE_SIZE)> arrTextureTypeToVKImageType;
    extern std::array<std::pair<TextureType, vk::ImageViewType>, static_cast<size_t>(TextureType::RANGE_SIZE)> arrTextureTypeToVKImageViewType;
}


#endif // !VG_TEXTURE_TYPE_H
