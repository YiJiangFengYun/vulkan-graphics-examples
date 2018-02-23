#include "graphics/texture/texture_cube.hpp"

namespace vg
{
	TextureCube::TextureCube(TextureFormat format, Bool32 mipMap, uint32_t size)
		:Texture(format, mipMap)
	{
		m_type = TextureType::CUBE;
		m_width = size;
		m_height = size;
		_init();
	}

	TextureCube::~TextureCube()
	{
	}

	uint32_t TextureCube::getSize() const
	{
		return m_width;
	}

	std::vector<Color> TextureCube::getPixels(CubemapFace face, uint32_t mipLevel) const
	{
		return _getPixels(static_cast<uint32_t>(face), mipLevel);
	}

	std::vector<Color32> TextureCube::getPixels32(CubemapFace face, uint32_t mipLevel) const
	{
		return _getPixels32(static_cast<uint32_t>(face), mipLevel);
	}

	void TextureCube::setPixels(const std::vector<Color> &colors, CubemapFace face, uint32_t mipLevel)
	{
		_setPixels(colors, static_cast<uint32_t>(face), mipLevel);
	}

	void TextureCube::setPixels32(const std::vector<Color32> &colors, CubemapFace face, uint32_t mipLevel)
	{
		_setPixels32(colors, static_cast<uint32_t>(face), mipLevel);
	}

	void TextureCube::setPixels(const void* colors, uint32_t size, CubemapFace face, uint32_t mipLevel)
	{
		_setPixels(colors, size, static_cast<uint32_t>(face), mipLevel);
	}

	void TextureCube::setPixels32(const void* colors, uint32_t size, CubemapFace face, uint32_t mipLevel)
	{
		_setPixels32(colors, size, static_cast<uint32_t>(face), mipLevel);
	}

	void TextureCube::apply(Bool32 updateMipmaps, Bool32 makeUnreadable)
	{
		_apply(updateMipmaps, makeUnreadable);
	}
}