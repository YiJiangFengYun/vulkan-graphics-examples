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
        DEPTH_ATTACHMENT,
        TEX_2D_COLOR_ATTACHMENT,
        TEX_2D_DEPTH_STENCIL_ATTACHMENT,
        TEX_2D_DEPTH_ATTACHMENT,
        CUBE_COLOR_ATTACHMENT,
        CUBE_DEPTH_STENCIL_ATTACHMENT,
        CUBE_DEPTH_ATTACHMENT,
        BEGIN_RANGE = TEX_1D,
        END_RANGE = CUBE_DEPTH_ATTACHMENT,
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

    enum SamplerTextureType {
        TEX_1D = vk::ImageViewType::e1D,
        TEX_2D = vk::ImageViewType::e2D,
        TEX_3D = vk::ImageViewType::e3D,
        TEX_CUBE = vk::ImageViewType::eCube,
        TEX_1D_ARRAY = vk::ImageViewType::e1DArray,
        TEX_2D_ARRAY = vk::ImageViewType::e2DArray,
        TEX_2D_CUBE_ARRAY = vk::ImageViewType::eCubeArray
    };

    extern vk::ImageViewType tranSamplerTextureTypeToVKImageViewType(SamplerTextureType type);

    extern std::array<std::pair<TextureType, std::string>, static_cast<size_t>(TextureType::RANGE_SIZE)> arrTextureTypeToNames;
    extern std::array<std::pair<TextureType, vk::ImageType>, static_cast<size_t>(TextureType::RANGE_SIZE)> arrTextureTypeToVKImageType;
    extern std::array<std::pair<TextureType, vk::ImageViewType>, static_cast<size_t>(TextureType::RANGE_SIZE)> arrTextureTypeToVKImageViewType;
    extern std::array<std::pair<TextureType, vk::ImageCreateFlags>, static_cast<size_t>(TextureType::RANGE_SIZE)> arrTextureTypeToImageCreateFlags;
    extern void checkTexImageSize(TextureType type, uint32_t width, uint32_t height, uint32_t depth);
    extern uint32_t getTexArrayLayers(TextureType type, uint32_t arrayLength);
}


#endif // !VG_TEXTURE_TYPE_H
