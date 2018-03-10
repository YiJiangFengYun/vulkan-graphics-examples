#include "graphics/texture/texture_color_attachment.hpp"

namespace vg
{
	TextureColorAttachment::TextureColorAttachment(TextureFormat format, uint32_t width, uint32_t height)
		:Texture(format, VG_FALSE)
	{
		m_type = TextureType::COLOR_ATTACHMENT;
		m_width = width;
		m_height = height;
		m_vkImageUsageFlags = vk::ImageUsageFlagBits::eColorAttachment;
		m_vkImageLayout = vk::ImageLayout::eColorAttachmentOptimal;
		m_vkImageAspectFlags = vk::ImageAspectFlagBits::eColor;
		_init();
	}

	TextureColorAttachment::~TextureColorAttachment()
	{
	}

	uint32_t TextureColorAttachment::getWidth() const
	{
		return m_width;
	}

	uint32_t TextureColorAttachment::getHeight() const
	{
		return m_height;
	}

	void TextureColorAttachment::_init()
	{
		Texture::_init();
		//Transform Image layout to final layout.
		auto pCommandBuffer = beginSingleTimeCommands();
		_tranImageLayout(pCommandBuffer, *m_pImage, m_currVkImageLayout, m_vkImageLayout,
			0, m_mipMapLevels, 0, m_arrayLayer);
		endSingleTimeCommands(pCommandBuffer);
		m_currVkImageLayout = m_vkImageLayout;
	}
} //namespace kgs