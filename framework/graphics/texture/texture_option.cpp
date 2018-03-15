#include "graphics/texture/texture_option.hpp"

namespace vg
{
	std::map<TextureType, std::string> mapTextureTypeToName =
	{
		{TextureType::TEX_1D, "Texture 1D"},
		{TextureType::TEX_1D_ARRAY, "Texture 1D Array"},
		{TextureType::TEX_2D, "Texture 2D"},
		{TextureType::TEX_2D_ARRAY, "Texture 2D Array"},
		{TextureType::TEX_3D, "Texture 3D"},
		{TextureType::CUBE, "Texture Cube"},
		{TextureType::CUBE_ARRARY, "Texture Cube Array"},
		{TextureType::COLOR_ATTACHMENT, "Texture Color Attachment"},
		{TextureType::DEPTH_STENCIL_ATTACHMENT, "Texture Depth Stencil Attachment"}

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
		std::pair<TextureType, vk::ImageType>(TextureType::DEPTH_STENCIL_ATTACHMENT, vk::ImageType::e2D)
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
		std::pair<TextureType, vk::ImageViewType>(TextureType::DEPTH_STENCIL_ATTACHMENT, vk::ImageViewType::e2D)
	};

	std::array<std::tuple<FilterMode, vk::Filter, vk::SamplerMipmapMode>, static_cast<size_t>(FilterMode::RANGE_SIZE)> arrFilerModeToVK =
	{
		std::tuple<FilterMode, vk::Filter, vk::SamplerMipmapMode>(FilterMode::NEAREST, vk::Filter::eNearest, vk::SamplerMipmapMode::eNearest),
		std::tuple<FilterMode, vk::Filter, vk::SamplerMipmapMode>(FilterMode::LINEAR, vk::Filter::eLinear, vk::SamplerMipmapMode::eNearest),
		std::tuple<FilterMode, vk::Filter, vk::SamplerMipmapMode>(FilterMode::TRILINEAR, vk::Filter::eLinear, vk::SamplerMipmapMode::eLinear)
	};

	std::array<std::pair<SamplerAddressMode, vk::SamplerAddressMode>, static_cast<size_t>(SamplerAddressMode::RANGE_SIZE)> arrSamplerAddressModeToVK = 
	{
		std::pair<SamplerAddressMode, vk::SamplerAddressMode>(SamplerAddressMode::REPEAT, vk::SamplerAddressMode::eRepeat),
		std::pair<SamplerAddressMode, vk::SamplerAddressMode>(SamplerAddressMode::MIRRORED_REPEAT, vk::SamplerAddressMode::eMirroredRepeat),
		std::pair<SamplerAddressMode, vk::SamplerAddressMode>(SamplerAddressMode::CLAMP_TO_EDGE, vk::SamplerAddressMode::eClampToEdge),
		std::pair<SamplerAddressMode, vk::SamplerAddressMode>(SamplerAddressMode::CLAMP_TO_BORDER, vk::SamplerAddressMode::eClampToBorder),
		std::pair<SamplerAddressMode, vk::SamplerAddressMode>(SamplerAddressMode::MIRROR_CLAMP_TO_EDGE, vk::SamplerAddressMode::eMirrorClampToEdge)
	};
}