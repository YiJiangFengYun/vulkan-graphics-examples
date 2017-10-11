#include "graphics/texture/texture_1d.hpp"

namespace kgs
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

	std::vector<Color> Texture1D::getPixels(uint32_t mipLevel)
	{
		return _getPixels(0u, mipLevel);
	}

	std::vector<Color32> Texture1D::getPixels32(uint32_t mipLevel)
	{
		return _getPixels32(0u, mipLevel);
	}

	void Texture1D::setPixels(std::vector<Color> colors, uint32_t mipLevel)
	{
		_setPixels(colors, 0u, mipLevel);
	}

	void Texture1D::setPixels32(std::vector<Color32> colors, uint32_t mipLevel)
	{
		_setPixels32(colors, 0u, mipLevel);
	}

	void Texture1D::apply(Bool32 updateMipmaps, Bool32 makeUnreadable)
	{
		_apply(updateMipmaps, makeUnreadable);
	}
}
