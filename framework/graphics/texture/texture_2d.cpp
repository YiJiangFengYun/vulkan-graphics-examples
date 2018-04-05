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


	Texture2DColorAttachment::Texture2DColorAttachment(vk::Format format, Bool32 mipMap, uint32_t width, uint32_t height)
		:Texture(format, mipMap)
		, BaseColorAttachment()
	{
		m_type = TextureType::TEX_2D_COLOR_ATTACHMENT;
		m_width = width;
		m_height = height;
		m_vkImageUsageFlags = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eColorAttachment;
		m_vkImageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		m_vkImageAspectFlags = vk::ImageAspectFlagBits::eColor;
		_init();
	}

	Texture2DColorAttachment::~Texture2DColorAttachment()
	{
	}

	uint32_t Texture2DColorAttachment::getWidth() const
	{
		return m_width;
	}

	uint32_t Texture2DColorAttachment::getHeight() const
	{
		return m_height;
	}

	void Texture2DColorAttachment::applyData(const TextureDataInfo &layoutInfo
			, const void *memory
			, uint32_t size
			, Bool32 cacheMemory
		    , Bool32 createMipmaps)
	{
		_applyData(layoutInfo, memory, size, cacheMemory, createMipmaps);
	}

	uint32_t Texture2DColorAttachment::getColorAttachmentWidth() const
	{
		return m_width;
	}
		
	uint32_t Texture2DColorAttachment::getColorAttachmentHeight() const
	{
		return m_height;
	}
	
	uint32_t Texture2DColorAttachment::getColorAttachmentLayers() const
	{
		return 1u;
	}

	vk::Format Texture2DColorAttachment::getColorAttachmentFormat() const
	{
		return m_vkFormat;
	}

	vk::ImageLayout Texture2DColorAttachment::getColorAttachmentLayout() const
	{
		return m_vkImageLayout;
	}

	vk::ImageView *Texture2DColorAttachment::getColorAttachmentImageView() const
	{
		return m_pImageView.get();
	}

	void Texture2DColorAttachment::_init()
	{
		Texture::_init();
		//Transform Image layout to final layout.
		auto pCommandBuffer = beginSingleTimeCommands();
		_tranImageLayout(pCommandBuffer, *m_pImage, m_usingVkImageLayout, m_vkImageLayout,
			0, m_mipMapLevels, 0, m_arrayLayer);
		endSingleTimeCommands(pCommandBuffer);
		m_usingVkImageLayout = m_vkImageLayout;
	}

	Texture2DDepthStencilAttachment::Texture2DDepthStencilAttachment(vk::Format format, Bool32 mipMap, uint32_t width, uint32_t height)
		:Texture(format, mipMap)
		, BaseDepthStencilAttachment()
	{
		m_type = TextureType::TEX_2D_DEPTH_STENCIL_ATTACHMENT;
		m_width = width;
		m_height = height;
		m_vkImageUsageFlags = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eDepthStencilAttachment;
		m_vkImageLayout = vk::ImageLayout::eDepthStencilReadOnlyOptimal;
		m_vkImageAspectFlags = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
		_checkDepthFormat();
		_init();
	}

	Texture2DDepthStencilAttachment::~Texture2DDepthStencilAttachment()
	{
	}

	uint32_t Texture2DDepthStencilAttachment::getWidth() const
	{
		return m_width;
	}

	uint32_t Texture2DDepthStencilAttachment::getHeight() const
	{
		return m_height;
	}

	uint32_t Texture2DDepthStencilAttachment::getDepthStencilAttachmentWidth() const
	{
		return m_width;
	}

	uint32_t Texture2DDepthStencilAttachment::getDepthStencilAttachmentHeight() const
	{
		return m_height;
	}
	
	uint32_t Texture2DDepthStencilAttachment::getDepthStencilAttachmentLayers() const
	{
		return 1u;
	}

	vk::Format Texture2DDepthStencilAttachment::getDepthStencilAttachmentFormat() const
	{
		return m_vkFormat;
	}

	vk::ImageLayout Texture2DDepthStencilAttachment::getDepthStencilAttachmentLayout() const
	{
		return m_vkImageLayout;
	}

	vk::ImageView *Texture2DDepthStencilAttachment::getDepthStencilAttachmentImageView() const
	{
		return m_pImageView.get();
	}

	void Texture2DDepthStencilAttachment::applyData(const TextureDataInfo &layoutInfo
			, const void *memory
			, uint32_t size
			, Bool32 cacheMemory
		    , Bool32 createMipmaps)
	{
		_applyData(layoutInfo, memory, size, cacheMemory, createMipmaps);
	}

	void Texture2DDepthStencilAttachment::_init()
	{
		Texture::_init();
		//Transform Image layout to final layout.
		auto pCommandBuffer = beginSingleTimeCommands();
		_tranImageLayout(pCommandBuffer, *m_pImage, m_usingVkImageLayout, m_vkImageLayout,
			0, m_mipMapLevels, 0, m_arrayLayer);
		endSingleTimeCommands(pCommandBuffer);
		m_usingVkImageLayout = m_vkImageLayout;
	}

	void Texture2DDepthStencilAttachment::_checkDepthFormat()
	{
		_updateVkFormat();
		const auto &pPhysicalDevice = pApp->getPhysicalDevice();
		const auto &props = pPhysicalDevice->getFormatProperties(m_vkFormat);
		if ((props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment) !=
			vk::FormatFeatureFlagBits::eDepthStencilAttachment)
		{
			throw std::runtime_error("Depth format is not supported!");
		}
	}
}