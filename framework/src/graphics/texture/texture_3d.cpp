#include "graphics/texture/texture_3d.hpp"

namespace kgs
{
	Texture3D::Texture3D(Device device, TextureFormat format, Bool32 mipMap, uint32_t width, uint32_t height, uint32_t depth)
		:Texture(device, format, mipMap)
	{
		m_type = TextureType::TEX_3D;
		m_width = width;
		m_height = height;
		m_depth = depth;
		_updateMipMapLevels();
		_updateArrayLayer();
		_updateVkFormat();
		_updateVkFilter();
		_updateVkSamplerAddressMode();
		_createImage();
		_createImageView();
		_createSampler();
	}

	Texture3D::~Texture3D()
	{
	}

	std::vector<Color> Texture3D::getPixels(uint32_t mipLevel)
	{
		return _getPixels(0u, mipLevel);
	}

	std::vector<Color32> Texture3D::getPixels32(uint32_t mipLevel)
	{
		return _getPixels32(0u, mipLevel);
	}

	void Texture3D::setPixels(std::vector<Color> colors, uint32_t mipLevel)
	{
		_setPixels(colors, 0u, mipLevel);
	}

	void Texture3D::setPixels32(std::vector<Color32> colors, uint32_t mipLevel)
	{
		_setPixels32(colors, 0u, mipLevel);
	}

	void Texture3D::apply(Bool32 updateMipmaps, Bool32 makeUnreadable)
	{
		_apply(updateMipmaps, makeUnreadable);
	}
}