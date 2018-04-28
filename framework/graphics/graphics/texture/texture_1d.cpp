#include "graphics/texture/texture_1d.hpp"

namespace vg
{
	Texture1D::Texture1D(vk::Format format
	    , Bool32 mipMap
		, uint32_t width
		, Bool32 defaultImageView
		, Bool32 defaultSampler
		)
		:Texture(format
		, mipMap
		, defaultImageView
		, defaultSampler
		)
	{
		m_type = TextureType::TEX_1D;
		m_width = width;
		m_allAspectFlags = vk::ImageAspectFlagBits::eColor;
		m_usageFlags = vk::ImageUsageFlagBits::eSampled;
		m_layout = vk::ImageLayout::eShaderReadOnlyOptimal;
		_init(VG_TRUE);
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
