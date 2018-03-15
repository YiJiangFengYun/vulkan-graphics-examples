#include "graphics/texture/texture_cube.hpp"

namespace vg
{
	TextureCube::TextureCube(vk::Format format, Bool32 mipMap, uint32_t width, uint32_t height)
		:Texture(format, mipMap)
	{
		m_type = TextureType::CUBE;
		m_width = width;
		m_height = height;
		_init();
	}

	TextureCube::~TextureCube()
	{
	}

	uint32_t TextureCube::getWidth() const
	{
		return m_width;
	}

	uint32_t TextureCube::getHeight() const
	{
		return m_height;
	}

	void TextureCube::applyData(const TextureDataLayout &layoutInfo
			, const void *memory
			, uint32_t size
			, Bool32 cacheMemory
		    , Bool32 createMipmaps)
	{
		_applyData(layoutInfo, memory, size, cacheMemory, createMipmaps);
	}
}