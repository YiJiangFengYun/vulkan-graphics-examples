#include "graphics/texture/texture_1d.hpp"

namespace kgs
{
	Texture1D::Texture1D(Device device, TextureFormat format, Bool32 mipMap, uint32_t width)
		:Texture(device, format, mipMap), m_width(width)
	{

	}

	Texture1D::~Texture1D()
	{
		
	}

	std::vector<Color> Texture1D::getPixels(uint32_t mipLevel)
	{
		return {};
	}

	std::vector<Color32> Texture1D::getPixels32(uint32_t mipLevel)
	{
		return {};
	}

	void Texture1D::setPixels(std::vector<Color> colors, uint32_t mipLevel)
	{

	}

	void Texture1D::setPixels32(std::vector<Color32> colors, uint32_t mipLevel)
	{

	}

	void Texture1D::apply(Bool32 updateMipmaps, Bool32 makeUnreadable)
	{

	}
}
