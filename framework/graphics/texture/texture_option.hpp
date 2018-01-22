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
		BEGIN_RANGE = TEX_1D,
		END_RANGE = DEPTH_STENCIL_ATTACHMENT,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};

	enum class TextureFormat
	{
		R8G8B8A8_UNORM,
		D32_SFLOAT_S8_UINT,
		BEGIN_RANGE = R8G8B8A8_UNORM,
		END_RANGE = D32_SFLOAT_S8_UINT,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};

	enum class FilterMode
	{
		NEAREST,
		LINEAR,
		TRILINEAR,
		BEGIN_RANGE = NEAREST,
		END_RANGE = TRILINEAR,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};

	enum class SamplerAddressMode
	{
		REPEAT,
		MIRRORED_REPEAT,
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER,
		MIRROR_CLAMP_TO_EDGE,
		BEGIN_RANGE = REPEAT,
		END_RANGE = MIRROR_CLAMP_TO_EDGE,
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
	extern std::array<std::pair<TextureFormat, vk::Format>, static_cast<size_t>(TextureFormat::RANGE_SIZE)> arrFormatToVKFormat;
	extern std::array<std::pair<TextureType, vk::ImageViewType>, static_cast<size_t>(TextureType::RANGE_SIZE)> arrTextureTypeToVKImageViewType;
	extern std::array<std::tuple<FilterMode, vk::Filter, vk::SamplerMipmapMode>, static_cast<size_t>(FilterMode::RANGE_SIZE)> arrFilerModeToVK;
	extern std::array<std::pair<SamplerAddressMode, vk::SamplerAddressMode>, static_cast<size_t>(SamplerAddressMode::RANGE_SIZE)> arrSamplerAddressModeToVK;
}


#endif // !VG_TEXTURE_TYPE_H
