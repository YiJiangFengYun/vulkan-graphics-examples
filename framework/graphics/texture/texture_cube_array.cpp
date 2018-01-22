#include "graphics/texture/texture_cube_array.hpp"

namespace vg
{
	TextureCubeArray::TextureCubeArray(TextureFormat format, Bool32 mipMap, uint32_t size, uint32_t arraylength)
		:Texture(format, mipMap)
	{
		m_type = TextureType::CUBE_ARRARY;
		m_width = size;
		m_height = size;
		m_arrayLength = arraylength;
		_init();
	}

	TextureCubeArray::~TextureCubeArray()
	{
	}

	uint32_t TextureCubeArray::getSize()
	{
		return m_width;
	}

	uint32_t TextureCubeArray::getArrayLength()
	{
		return m_arrayLength;
	}

	std::vector<Color> TextureCubeArray::getPixels(CubemapFace face, uint32_t arrayIndex, uint32_t mipLevel)
	{
		return _getPixels(static_cast<uint32_t>(face) + arrayIndex * static_cast<uint32_t>(CubemapFace::RANGE_SIZE), mipLevel);
	}

	std::vector<Color32> TextureCubeArray::getPixels32(CubemapFace face, uint32_t arrayIndex, uint32_t mipLevel)
	{
		return _getPixels32(static_cast<uint32_t>(face) + arrayIndex * static_cast<uint32_t>(CubemapFace::RANGE_SIZE), mipLevel);
	}

	void TextureCubeArray::setPixels(std::vector<Color> colors, CubemapFace face, uint32_t arrayIndex, uint32_t mipLevel)
	{
		_setPixels(colors, static_cast<uint32_t>(face) + arrayIndex * static_cast<uint32_t>(CubemapFace::RANGE_SIZE), mipLevel);
	}

	void TextureCubeArray::setPixels32(std::vector<Color32> colors, CubemapFace face, uint32_t arrayIndex, uint32_t mipLevel)
	{
		_setPixels32(colors, static_cast<uint32_t>(face) + arrayIndex * static_cast<uint32_t>(CubemapFace::RANGE_SIZE), mipLevel);
	}

	void TextureCubeArray::apply(Bool32 updateMipmaps, Bool32 makeUnreadable)
	{
		_apply(updateMipmaps, makeUnreadable);
	}
}