#include "graphics/texture/texture_1d.hpp"

namespace vg
{
	Texture1D::Texture1D(TextureFormat format, Bool32 mipMap, uint32_t width)
		:Texture(format, mipMap)
	{
		m_type = TextureType::TEX_1D;
		m_width = width;
		_init();
	}

	Texture1D::~Texture1D()
	{
	}

	uint32_t Texture1D::getWidth() const
	{
		return m_width;
	}

	std::vector<Color> Texture1D::getPixels(uint32_t mipLevel) const
	{
		return _getPixels(0u, mipLevel);
	}

	std::vector<Color32> Texture1D::getPixels32(uint32_t mipLevel) const
	{
		return _getPixels32(0u, mipLevel);
	}

	void Texture1D::setPixels(const std::vector<Color> &colors, uint32_t mipLevel)
	{
		_setPixels(colors, 0u, mipLevel);
	}

	void Texture1D::setPixels32(const std::vector<Color32> &colors, uint32_t mipLevel)
	{
		_setPixels32(colors, 0u, mipLevel);
	}

	void Texture1D::setPixels(const void* colors, uint32_t size, uint32_t mipLevel)
	{
		_setPixels(colors, size, 0u, mipLevel);
	}

	void Texture1D::setPixels32(const void* colors, uint32_t size, uint32_t mipLevel)
	{
		_setPixels32(colors, size, 0u, mipLevel);
	}

	void Texture1D::apply(Bool32 updateMipmaps, Bool32 makeUnreadable)
	{
		_apply(updateMipmaps, makeUnreadable);
	}
}
