#include "graphics/texture/texture_cube.hpp"

namespace vg
{
	TextureCube::TextureCube(vk::Format format, Bool32 mipMap, uint32_t size)
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

	void TextureCube::applyData(const TextureDataLayout &layoutInfo
			, const void *memory
			, uint32_t size
			, Bool32 cacheMemory
		    , Bool32 createMipmaps)
	{
		_applyData(layoutInfo, memory, size, cacheMemory, createMipmaps);
	}
}