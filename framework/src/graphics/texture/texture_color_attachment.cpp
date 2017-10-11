#include "graphics/texture/texture_color_attachment.hpp"

namespace kgs
{
	TextureColorAttachment::TextureColorAttachment(TextureFormat format, uint32_t width, uint32_t height)
		:Texture(format, KGS_FALSE)
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
} //namespace kgs