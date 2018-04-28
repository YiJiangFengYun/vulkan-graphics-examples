#ifndef VG_TEX_COLOR_ATTACHMENT_H
#define VG_TEX_COLOR_ATTACHMENT_H

#include "graphics/texture/texture.hpp"
#include "graphics/texture/attachment.hpp"

namespace vg
{
	class TextureColorAttachment : public Texture, public BaseColorAttachment
	{
	public:
		TextureColorAttachment(vk::Format format
		    , uint32_t width
			, uint32_t height
			, Bool32 isInputUsage = VG_FALSE
			, Bool32 defaultImageView = VG_TRUE
			, Bool32 defaultSampler = VG_TRUE
			);
		~TextureColorAttachment();
		uint32_t getWidth() const;
		uint32_t getHeight() const;

		virtual uint32_t getColorAttachmentWidth() const override;
		virtual uint32_t getColorAttachmentHeight() const override;
		virtual uint32_t getColorAttachmentLayers() const override;
		virtual const vk::Format getColorAttachmentFormat() const override;
		virtual const vk::ImageLayout getColorAttachmentLayout() const override;
		virtual const vk::ImageView *getColorAttachmentImageView() const override;
	private:
		TextureColorAttachment() = delete;
		virtual void _init(Bool32 importContent) override;
	};
} //namespace kgs

#endif // !VG_TEX_COLOR_ATTACHMENT_H
