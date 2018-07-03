#include "graphics/texture/texture_option.hpp"

namespace vg
{
    std::array<std::pair<TextureType, std::string>, static_cast<size_t>(TextureType::RANGE_SIZE)> arrTextureTypeToNames =
    {
        std::pair<TextureType, std::string>{TextureType::TEX_1D, "Texture 1D"},
        std::pair<TextureType, std::string>{TextureType::TEX_1D_ARRAY, "Texture 1D Array"},
        std::pair<TextureType, std::string>{TextureType::TEX_2D, "Texture 2D"},
        std::pair<TextureType, std::string>{TextureType::TEX_2D_ARRAY, "Texture 2D Array"},
        std::pair<TextureType, std::string>{TextureType::TEX_3D, "Texture 3D"},
        std::pair<TextureType, std::string>{TextureType::CUBE, "Texture Cube"},
        std::pair<TextureType, std::string>{TextureType::CUBE_ARRARY, "Texture Cube Array"},
        std::pair<TextureType, std::string>{TextureType::COLOR_ATTACHMENT, "Texture Color Attachment"},
        std::pair<TextureType, std::string>{TextureType::DEPTH_STENCIL_ATTACHMENT, "Texture Depth Stencil Attachment"},
        std::pair<TextureType, std::string>{TextureType::TEX_2D_COLOR_ATTACHMENT, "Texture 2D And Color Attachment"},
        std::pair<TextureType, std::string>{TextureType::TEX_2D_DEPTH_STENCIL_ATTACHMENT, "Texture 2D and Depth Stencil Attachment"},
        std::pair<TextureType, std::string>{TextureType::TEX_2D_DEPTH_ATTACHMENT, "Texture 2D and Depth Attachment"},
        std::pair<TextureType, std::string>{TextureType::CUBE_COLOR_ATTACHMENT, "Texture Cube and Color Attachment"},
        std::pair<TextureType, std::string>{TextureType::CUBE_DEPTH_STENCIL_ATTACHMENT, "Texture Cube and Depth Stencil Attachment"},
        std::pair<TextureType, std::string>{TextureType::CUBE_DEPTH_ATTACHMENT, "Texture Cube and Depth Attachment"},
    };

    std::array<std::pair<TextureType, vk::ImageType>, static_cast<size_t>(TextureType::RANGE_SIZE)> arrTextureTypeToVKImageType = {
        std::pair<TextureType, vk::ImageType>(TextureType::TEX_1D, vk::ImageType::e1D),
        std::pair<TextureType, vk::ImageType>(TextureType::TEX_1D_ARRAY, vk::ImageType::e1D),
        std::pair<TextureType, vk::ImageType>(TextureType::TEX_2D, vk::ImageType::e2D),
        std::pair<TextureType, vk::ImageType>(TextureType::TEX_2D_ARRAY, vk::ImageType::e2D),
        std::pair<TextureType, vk::ImageType>(TextureType::TEX_3D, vk::ImageType::e3D),
        std::pair<TextureType, vk::ImageType>(TextureType::CUBE, vk::ImageType::e2D),
        std::pair<TextureType, vk::ImageType>(TextureType::CUBE_ARRARY, vk::ImageType::e2D),
        std::pair<TextureType, vk::ImageType>(TextureType::COLOR_ATTACHMENT, vk::ImageType::e2D),
        std::pair<TextureType, vk::ImageType>(TextureType::DEPTH_STENCIL_ATTACHMENT, vk::ImageType::e2D),
        std::pair<TextureType, vk::ImageType>(TextureType::TEX_2D_COLOR_ATTACHMENT, vk::ImageType::e2D),
        std::pair<TextureType, vk::ImageType>(TextureType::TEX_2D_DEPTH_STENCIL_ATTACHMENT, vk::ImageType::e2D),
        std::pair<TextureType, vk::ImageType>(TextureType::TEX_2D_DEPTH_ATTACHMENT, vk::ImageType::e2D),
        std::pair<TextureType, vk::ImageType>(TextureType::CUBE_COLOR_ATTACHMENT, vk::ImageType::e2D),
        std::pair<TextureType, vk::ImageType>(TextureType::CUBE_DEPTH_STENCIL_ATTACHMENT, vk::ImageType::e2D),
        std::pair<TextureType, vk::ImageType>(TextureType::CUBE_DEPTH_ATTACHMENT, vk::ImageType::e2D),
    };

    std::array<std::pair<TextureType, vk::ImageViewType>, static_cast<size_t>(TextureType::RANGE_SIZE)> arrTextureTypeToVKImageViewType = {
        std::pair<TextureType, vk::ImageViewType>(TextureType::TEX_1D, vk::ImageViewType::e1D),
        std::pair<TextureType, vk::ImageViewType>(TextureType::TEX_1D_ARRAY, vk::ImageViewType::e1DArray),
        std::pair<TextureType, vk::ImageViewType>(TextureType::TEX_2D, vk::ImageViewType::e2D),
        std::pair<TextureType, vk::ImageViewType>(TextureType::TEX_2D_ARRAY, vk::ImageViewType::e2DArray),
        std::pair<TextureType, vk::ImageViewType>(TextureType::TEX_3D, vk::ImageViewType::e3D),
        std::pair<TextureType, vk::ImageViewType>(TextureType::CUBE, vk::ImageViewType::eCube),
        std::pair<TextureType, vk::ImageViewType>(TextureType::CUBE_ARRARY, vk::ImageViewType::eCubeArray),
        std::pair<TextureType, vk::ImageViewType>(TextureType::COLOR_ATTACHMENT, vk::ImageViewType::e2D),
        std::pair<TextureType, vk::ImageViewType>(TextureType::DEPTH_STENCIL_ATTACHMENT, vk::ImageViewType::e2D),
        std::pair<TextureType, vk::ImageViewType>(TextureType::TEX_2D_COLOR_ATTACHMENT, vk::ImageViewType::e2D),
        std::pair<TextureType, vk::ImageViewType>(TextureType::TEX_2D_DEPTH_STENCIL_ATTACHMENT, vk::ImageViewType::e2D),
        std::pair<TextureType, vk::ImageViewType>(TextureType::TEX_2D_DEPTH_ATTACHMENT, vk::ImageViewType::e2D),
        std::pair<TextureType, vk::ImageViewType>(TextureType::CUBE_COLOR_ATTACHMENT, vk::ImageViewType::eCube),
        std::pair<TextureType, vk::ImageViewType>(TextureType::CUBE_DEPTH_STENCIL_ATTACHMENT, vk::ImageViewType::eCube),
        std::pair<TextureType, vk::ImageViewType>(TextureType::CUBE_DEPTH_ATTACHMENT, vk::ImageViewType::eCube),
    };

    std::array<std::pair<TextureType, vk::ImageCreateFlags>, static_cast<size_t>(TextureType::RANGE_SIZE)> arrTextureTypeToImageCreateFlags = {
        std::pair<TextureType, vk::ImageCreateFlags>(TextureType::TEX_1D, vk::ImageCreateFlags()),
        std::pair<TextureType, vk::ImageCreateFlags>(TextureType::TEX_1D_ARRAY, vk::ImageCreateFlags()),
        std::pair<TextureType, vk::ImageCreateFlags>(TextureType::TEX_2D, vk::ImageCreateFlags()),
        std::pair<TextureType, vk::ImageCreateFlags>(TextureType::TEX_2D_ARRAY, vk::ImageCreateFlags()),
        std::pair<TextureType, vk::ImageCreateFlags>(TextureType::TEX_3D, vk::ImageCreateFlags()),
        std::pair<TextureType, vk::ImageCreateFlags>(TextureType::CUBE, vk::ImageCreateFlagBits::eCubeCompatible),
        std::pair<TextureType, vk::ImageCreateFlags>(TextureType::CUBE_ARRARY, vk::ImageCreateFlagBits::eCubeCompatible),
        std::pair<TextureType, vk::ImageCreateFlags>(TextureType::COLOR_ATTACHMENT, vk::ImageCreateFlags()),
        std::pair<TextureType, vk::ImageCreateFlags>(TextureType::DEPTH_STENCIL_ATTACHMENT, vk::ImageCreateFlags()),
        std::pair<TextureType, vk::ImageCreateFlags>(TextureType::TEX_2D_COLOR_ATTACHMENT, vk::ImageCreateFlags()),
        std::pair<TextureType, vk::ImageCreateFlags>(TextureType::TEX_2D_DEPTH_STENCIL_ATTACHMENT, vk::ImageCreateFlags()),
        std::pair<TextureType, vk::ImageCreateFlags>(TextureType::TEX_2D_DEPTH_ATTACHMENT, vk::ImageCreateFlags()),
        std::pair<TextureType, vk::ImageCreateFlags>(TextureType::CUBE_COLOR_ATTACHMENT, vk::ImageCreateFlagBits::eCubeCompatible),
        std::pair<TextureType, vk::ImageCreateFlags>(TextureType::CUBE_DEPTH_STENCIL_ATTACHMENT, vk::ImageCreateFlagBits::eCubeCompatible),
        std::pair<TextureType, vk::ImageCreateFlags>(TextureType::CUBE_DEPTH_ATTACHMENT, vk::ImageCreateFlagBits::eCubeCompatible),
    };

    uint32_t getTexArrayLayers(TextureType type, uint32_t arrayLength)
    {
        uint32_t arraylayers;
        switch (type)
        {
        case TextureType::TEX_1D:
        case TextureType::TEX_2D:
        case TextureType::TEX_2D_COLOR_ATTACHMENT:
        case TextureType::TEX_2D_DEPTH_STENCIL_ATTACHMENT:
        case TextureType::TEX_2D_DEPTH_ATTACHMENT:
        case TextureType::TEX_3D:
        case TextureType::COLOR_ATTACHMENT:
        case TextureType::DEPTH_STENCIL_ATTACHMENT:
        {
            arraylayers = 1U;
            break;
        }
        case TextureType::TEX_1D_ARRAY:
        case TextureType::TEX_2D_ARRAY:
        {
            arraylayers = arrayLength;
            break;
        }
        case TextureType::CUBE:
        case TextureType::CUBE_COLOR_ATTACHMENT:
        case TextureType::CUBE_DEPTH_STENCIL_ATTACHMENT:
        case TextureType::CUBE_DEPTH_ATTACHMENT:
        {
            arraylayers = static_cast<uint32_t>(CubemapFace::RANGE_SIZE);
            break;
        }
        case TextureType::CUBE_ARRARY:
        {
            arraylayers = static_cast<uint32_t>(CubemapFace::RANGE_SIZE) * arrayLength;
            break;
        }
        default:
            throw std::logic_error("Lack of caculating compatible array layer and flag at creating iamge for texture when texture type is " + 
                arrTextureTypeToNames[static_cast<size_t>(type)].second);
            break;
        }
        return arraylayers;
    }

    void checkTexImageSize(TextureType type, uint32_t width, uint32_t height, uint32_t depth)
    {
        switch (type)
        {
        case TextureType::TEX_1D:
        case TextureType::TEX_1D_ARRAY:
        {
            if (width == 0) throw std::invalid_argument("Invalid width argument at creating image for texture.");
            if (height != 1) throw std::invalid_argument("Invalid height argument at creating image for texture.");
            if (depth != 1) throw std::invalid_argument("Invalide depth argument at creating image for texture.");
            break;
        }
        case TextureType::TEX_2D:
        case TextureType::TEX_2D_ARRAY:
        case TextureType::COLOR_ATTACHMENT:
        case TextureType::DEPTH_STENCIL_ATTACHMENT:
        case TextureType::TEX_2D_COLOR_ATTACHMENT:
        case TextureType::TEX_2D_DEPTH_STENCIL_ATTACHMENT:
        case TextureType::TEX_2D_DEPTH_ATTACHMENT:
        {
            if (width == 0) throw std::invalid_argument("Invalid width argument at creating image for texture.");
            if (height == 0) throw std::invalid_argument("Invalid height argument at creating image for texture.");
            if (depth != 1) throw std::invalid_argument("Invalide depth argument at creating image for texture.");
            break;
        }
        case TextureType::CUBE:
        case TextureType::CUBE_COLOR_ATTACHMENT:
        case TextureType::CUBE_DEPTH_STENCIL_ATTACHMENT:
        case TextureType::CUBE_DEPTH_ATTACHMENT:
        case TextureType::CUBE_ARRARY:
        {
            if (width == 0) throw std::invalid_argument("Invalid width argument at creating image for texture.");
            if (height == 0) throw std::invalid_argument("Invalid height argument at creating image for texture.");
            if (depth != 1) throw std::invalid_argument("Invalide depth argument at creating image for texture.");
            break;
        }
        case TextureType::TEX_3D:
        {
            if (width == 0) throw std::invalid_argument("Invalid width argument at creating image for texture.");
            if (height == 0) throw std::invalid_argument("Invalid height argument at creating image for texture.");
            if (depth == 0) throw std::invalid_argument("Invalid depth argument at creating image for texture.");
            break;
        }
        default:
            throw std::logic_error("Lack of checking argument at creating iamge for texture when texture type is " + 
                arrTextureTypeToNames[static_cast<size_t>(type)].second);
            break;
        }
    }
}