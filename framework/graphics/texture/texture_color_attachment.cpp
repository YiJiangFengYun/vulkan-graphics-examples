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

	uint32_t TextureColorAttachment::getWidth()
	{
		return m_width;
	}

	uint32_t TextureColorAttachment::getHeight()
	{
		return m_height;
	}
} //namespace kgs