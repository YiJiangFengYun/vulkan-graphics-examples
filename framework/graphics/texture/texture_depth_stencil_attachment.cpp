#include "graphics/texture/texture_depth_stencil_attachment.hpp"

namespace vg
{
	TextureDepthStencilAttachment::TextureDepthStencilAttachment(vk::Format format, uint32_t width, uint32_t height)
		:Texture(format, VG_FALSE)
	{
		m_type = TextureType::DEPTH_STENCIL_ATTACHMENT;
		m_width = width;
		m_height = height;
		m_vkImageUsageFlags = vk::ImageUsageFlagBits::eDepthStencilAttachment;
		m_vkImageLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
		m_vkImageAspectFlags = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
		_init();
	}

	TextureDepthStencilAttachment::~TextureDepthStencilAttachment()
	{
	}

	uint32_t TextureDepthStencilAttachment::getWidth() const
	{
		return m_width;
	}

	uint32_t TextureDepthStencilAttachment::getHeight() const
	{
		return m_height;
	}

	uint32_t TextureDepthStencilAttachment::getDepthStencilAttachmentWidth() const
	{
		return m_width;
	}

	uint32_t TextureDepthStencilAttachment::getDepthStencilAttachmentHeight() const
	{
		return m_height;
	}
	
	uint32_t TextureDepthStencilAttachment::getDepthStencilAttachmentLayers() const
	{
		return 1u;
	}

	vk::Format TextureDepthStencilAttachment::getDepthStencilAttachmentFormat() const
	{
		return m_vkFormat;
	}

	vk::ImageLayout TextureDepthStencilAttachment::getDepthStencilAttachmentLayout() const
	{
		return m_vkImageLayout;
	}

	vk::ImageView *TextureDepthStencilAttachment::getDepthStencilAttachmentImageView() const
	{
		return m_pImageView.get();
	}

	void TextureDepthStencilAttachment::_init()
	{
		_checkDepthFormat();
		Texture::_init();
		//Transform Image layout to final layout.
		auto pCommandBuffer = beginSingleTimeCommands();
		_tranImageLayout(pCommandBuffer, *m_pImage, m_usingVkImageLayout, m_vkImageLayout,
			0, m_mipMapLevels, 0, m_arrayLayer);
		endSingleTimeCommands(pCommandBuffer);
		m_usingVkImageLayout = m_vkImageLayout;
	}

	void TextureDepthStencilAttachment::_checkDepthFormat()
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
} //namespace kgs