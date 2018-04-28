#ifndef VG_TEX_DEPTH_STENCIL_ATTACHMENT_H
#define VG_TEX_DEPTH_STENCIL_ATTACHMENT_H

#include "graphics/texture/texture.hpp"
#include "graphics/texture/attachment.hpp"

namespace vg
{
	class TextureDepthStencilAttachment : public Texture, public BaseDepthStencilAttachment
	{
	public:
		TextureDepthStencilAttachment(vk::Format format
		    , uint32_t width
			, uint32_t height
			, Bool32 isInputUsage = VG_FALSE
			, Bool32 defaultImageView = VG_TRUE
			, Bool32 defaultSampler = VG_TRUE
			);
		~TextureDepthStencilAttachment();
		uint32_t getWidth() const;
		uint32_t getHeight() const;
		virtual uint32_t getDepthStencilAttachmentWidth() const override;
		virtual uint32_t getDepthStencilAttachmentHeight() const override;
		virtual uint32_t getDepthStencilAttachmentLayers() const override;
		virtual const vk::Format getDepthStencilAttachmentFormat() const override;
		virtual const vk::ImageLayout getDepthStencilAttachmentLayout() const override;
		virtual const vk::ImageView *getDepthStencilAttachmentImageView() const override;
	private:
		TextureDepthStencilAttachment() = delete;
		virtual void _init(Bool32 importContent) override;
		void _checkDepthFormat();
	};
} //namespace kgs

#endif // !VG_TEX_DEPTH_STENCIL_ATTACHMENT_H