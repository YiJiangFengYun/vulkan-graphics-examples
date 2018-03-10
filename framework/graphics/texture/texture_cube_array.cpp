#include "graphics/texture/texture_cube_array.hpp"

namespace vg
{
	TextureCubeArray::TextureCubeArray(TextureFormat format, Bool32 mipMap, uint32_t size, uint32_t arraylength)
		:Texture(format, mipMap)
	{
		m_type = TextureType::CUBE_ARRARY;
		m_width = size;
		m_height = size;
		m_arrayLength = arraylength;
		_init();
	}

	TextureCubeArray::~TextureCubeArray()
	{
	}

	uint32_t TextureCubeArray::getSize() const
	{
		return m_width;
	}

	uint32_t TextureCubeArray::getArrayLength() const
	{
		return m_arrayLength;
	}

	void TextureCubeArray::applyData(TextureDataLayout layoutInfo
			, const void *memory
			, uint32_t size
			, Bool32 cacheMemory
		    , Bool32 createMipmaps)
	{
		_applyData(layoutInfo, memory, size, cacheMemory, createMipmaps);
	}
}