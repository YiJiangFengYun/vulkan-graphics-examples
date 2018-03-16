#include "graphics/texture/texture_3d.hpp"

namespace vg
{
	Texture3D::Texture3D(vk::Format format, Bool32 mipMap, uint32_t width, uint32_t height, uint32_t depth)
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

	uint32_t Texture3D::getWidth() const
	{
		return m_width;
	}
 
	uint32_t Texture3D::getHeight() const
	{
		return m_height;
	}

	uint32_t Texture3D::getDepth() const
	{
		return m_depth;
	}

	void Texture3D::applyData(const TextureDataInfo &layoutInfo
			, const void *memory
			, uint32_t size
			, Bool32 cacheMemory
		    , Bool32 createMipmaps)
	{
		_applyData(layoutInfo, memory, size, cacheMemory, createMipmaps);
	}
}