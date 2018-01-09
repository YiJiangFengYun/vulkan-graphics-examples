#include "graphics/texture/texture_3d.hpp"

namespace kgs
{
	Texture3D::Texture3D(TextureFormat format, Bool32 mipMap, uint32_t width, uint32_t height, uint32_t depth)
		:Texture(format, mipMap)
	{
		m_type = TextureType::TEX_3D;
		m_width = width;
		m_height = height;
		m_depth = depth;
		_init();
	}

	Texture3D::~Texture3D()
	{
	}

	uint32_t Texture3D::getWidth()
	{
		return m_width;
	}

	uint32_t Texture3D::getHeight()
	{
		return m_height;
	}

	uint32_t Texture3D::getDepth()
	{
		return m_depth;
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