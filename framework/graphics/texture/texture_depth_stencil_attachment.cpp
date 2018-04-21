#include "graphics/texture/texture_depth_stencil_attachment.hpp"

namespace vg
{
	TextureDepthStencilAttachment::TextureDepthStencilAttachment(vk::Format format
	    , uint32_t width
		, uint32_t height
		, Bool32 isInputUsage
		, Bool32 defaultImageView
		, Bool32 defaultSampler
		)
		: Texture(format
		, VG_FALSE
		, defaultImageView
		, defaultSampler
		)
		, BaseDepthStencilAttachment(isInputUsage)
	{
		m_type = TextureType::DEPTH_STENCIL_ATTACHMENT;
		m_width = width;
		m_height = height;
		m_allAspectFlags = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
		m_usageFlags = vk::ImageUsageFlagBits::eDepthStencilAttachment;
		m_layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
		if (isInputUsage)
		{
			m_usageFlags |= vk::ImageUsageFlagBits::eInputAttachment;
		}
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

	const vk::Format TextureDepthStencilAttachment::getDepthStencilAttachmentFormat() const
	{
		return m_format;
	}

	const vk::ImageLayout TextureDepthStencilAttachment::getDepthStencilAttachmentLayout() const
	{
		return m_layout;
	}

	const vk::ImageView *TextureDepthStencilAttachment::getDepthStencilAttachmentImageView() const
	{
		return m_pImageView->getImageView();
	}

	void TextureDepthStencilAttachment::_init()
	{
		_checkDepthFormat();
		Texture::_init();
	}

	void TextureDepthStencilAttachment::_checkDepthFormat()
	{
		const auto &pPhysicalDevice = pApp->getPhysicalDevice();
		const auto &props = pPhysicalDevice->getFormatProperties(m_format);
		if ((props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment) !=
			vk::FormatFeatureFlagBits::eDepthStencilAttachment)
		{
			throw std::runtime_error("The format don't support as depth stencil attachment!");
		}
	}
} //namespace kgs