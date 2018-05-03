#include "graphics/texture/texture_color_attachment.hpp"

namespace vg
{
	TextureColorAttachment::TextureColorAttachment(vk::Format format
	    , uint32_t width
		, uint32_t height
		, Bool32 isInputUsage
		, vk::ImageUsageFlags additionalUsage
		, Bool32 defaultImageView
		, Bool32 defaultSampler
		)
		:Texture(format
		, VG_FALSE
		, additionalUsage
		, defaultImageView
		, defaultSampler
		)
		, BaseColorAttachment(isInputUsage)
	{
		m_type = TextureType::COLOR_ATTACHMENT;
		m_width = width;
		m_height = height;
		m_allAspectFlags = vk::ImageAspectFlagBits::eColor;
		m_usageFlags = vk::ImageUsageFlagBits::eColorAttachment;
		m_layout = vk::ImageLayout::eColorAttachmentOptimal;

		if (isInputUsage)
		{
			m_usageFlags |= vk::ImageUsageFlagBits::eInputAttachment;
		}
		_init(VG_FALSE);
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

	uint32_t TextureColorAttachment::getColorAttachmentWidth() const
	{
		return m_width;
	}
		
	uint32_t TextureColorAttachment::getColorAttachmentHeight() const
	{
		return m_height;
	}
	
	uint32_t TextureColorAttachment::getColorAttachmentLayers() const
	{
		return 1u;
	}

	const vk::Format TextureColorAttachment::getColorAttachmentFormat() const
	{
		return m_format;
	}

	const vk::ImageLayout TextureColorAttachment::getColorAttachmentLayout() const
	{
		return m_layout;
	}

	const vk::ImageView *TextureColorAttachment::getColorAttachmentImageView() const
	{
		return m_pImageView->getImageView();
	}

	void TextureColorAttachment::_init(Bool32 importContent)
	{
		Texture::_init(importContent);
	}
} //namespace kgs