#include "graphics/texture/texture_2d.hpp"

namespace vg
{
	Texture2D::Texture2D(TextureFormat format, Bool32 mipMap, uint32_t width, uint32_t height)
		:Texture(format, mipMap)
	{
		m_type = TextureType::TEX_2D;
		m_width = width;
		m_height = height;
		_init();
	}

	Texture2D::~Texture2D()
	{
	}

	uint32_t Texture2D::getWidth() const
	{
		return m_width;
	}

	uint32_t Texture2D::getHeight() const
	{
		return m_height;
	}

	std::vector<Color> Texture2D::getPixels(uint32_t mipLevel) const
	{
		return _getPixels(0u, mipLevel);
	}

	std::vector<Color32> Texture2D::getPixels32(uint32_t mipLevel) const
	{
		return _getPixels32(0u, mipLevel);
	}

	void Texture2D::setPixels(const std::vector<Color> &colors, uint32_t mipLevel)
	{
		_setPixels(colors, 0u, mipLevel);
	}

	void Texture2D::setPixels32(const std::vector<Color32> &colors, uint32_t mipLevel)
	{
		_setPixels32(colors, 0u, mipLevel);
	}

	void Texture2D::setPixels(const void* colors, uint32_t size, uint32_t mipLevel)
	{
		_setPixels(colors, size, 0u, mipLevel);
	}

	void Texture2D::setPixels32(const void* colors, uint32_t size, uint32_t mipLevel)
	{
		_setPixels32(colors, size, 0u, mipLevel);
	}


	void Texture2D::apply(Bool32 updateMipmaps, Bool32 makeUnreadable)
	{
		_apply(updateMipmaps, makeUnreadable);
	}
}