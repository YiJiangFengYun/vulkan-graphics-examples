#include "graphics/texture/texture_2d.hpp"

namespace vg
{
	Texture2D::Texture2D(vk::Format format, Bool32 mipMap, uint32_t width, uint32_t height)
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

	void Texture2D::applyData(const TextureDataInfo &layoutInfo
			, const void *memory
			, uint32_t size
			, Bool32 cacheMemory
		    , Bool32 createMipmaps)
	{
		_applyData(layoutInfo, memory, size, cacheMemory, createMipmaps);
	}
}