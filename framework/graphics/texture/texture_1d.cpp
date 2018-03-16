#include "graphics/texture/texture_1d.hpp"

namespace vg
{
	Texture1D::Texture1D(vk::Format format, Bool32 mipMap, uint32_t width)
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

	void Texture1D::applyData(const TextureDataInfo &layoutInfo
			, const void *memory
			, uint32_t size
			, Bool32 cacheMemory
		    , Bool32 createMipmaps)
	{
		_applyData(layoutInfo, memory, size, cacheMemory, createMipmaps);
	}
}
