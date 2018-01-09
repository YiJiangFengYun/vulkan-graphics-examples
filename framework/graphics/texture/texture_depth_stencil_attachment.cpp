#include "graphics/texture/texture_depth_stencil_attachment.hpp"

namespace kgs
{
	TextureDepthStencilAttachment::TextureDepthStencilAttachment(TextureFormat format, uint32_t width, uint32_t height)
		:Texture(format, KGS_FALSE)
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

	uint32_t TextureDepthStencilAttachment::getWidth()
	{
		return m_width;
	}

	uint32_t TextureDepthStencilAttachment::getHeight()
	{
		return m_height;
	}
} //namespace kgs