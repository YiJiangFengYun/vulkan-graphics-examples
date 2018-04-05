#ifndef VG_TEX_COLOR_ATTACHMENT_H
#define VG_TEX_COLOR_ATTACHMENT_H

#include "graphics/texture/texture.hpp"
#include "graphics/texture/attachment.hpp"

namespace vg
{
	class TextureColorAttachment : public Texture, public BaseColorAttachment
	{
	public:
		TextureColorAttachment(vk::Format format, uint32_t width, uint32_t height);
		~TextureColorAttachment();
		uint32_t getWidth() const;
		uint32_t getHeight() const;

		virtual uint32_t getColorAttachmentWidth() const override;
		virtual uint32_t getColorAttachmentHeight() const override;
		virtual uint32_t getColorAttachmentLayers() const override;
		virtual vk::Format getColorAttachmentFormat() const override;
		virtual vk::ImageLayout getColorAttachmentLayout() const override;
		virtual vk::ImageView *getColorAttachmentImageView() const override;
	private:
		TextureColorAttachment() = delete;
		virtual void _init() override;
	};
} //namespace kgs

#endif // !VG_TEX_COLOR_ATTACHMENT_H
