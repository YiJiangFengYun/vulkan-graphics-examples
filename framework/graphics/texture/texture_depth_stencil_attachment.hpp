#ifndef VG_TEX_DEPTH_STENCIL_ATTACHMENT_H
#define VG_TEX_DEPTH_STENCIL_ATTACHMENT_H

#include "graphics/texture/texture.hpp"
#include "graphics/texture/attachment.hpp"

namespace vg
{
	class TextureDepthStencilAttachment : public Texture, public BaseDepthStencilAttachment
	{
	public:
		TextureDepthStencilAttachment(vk::Format format, uint32_t width, uint32_t height);
		~TextureDepthStencilAttachment();
		uint32_t getWidth() const;
		uint32_t getHeight() const;
		virtual uint32_t getDepthStencilAttachmentWidth() const override;
		virtual uint32_t getDepthStencilAttachmentHeight() const override;
		virtual uint32_t getDepthStencilAttachmentLayers() const override;
		virtual vk::Format getDepthStencilAttachmentFormat() const override;
		virtual vk::ImageLayout getDepthStencilAttachmentLayout() const override;
		virtual vk::ImageView *getDepthStencilAttachmentImageView() const override;
	private:
		TextureDepthStencilAttachment() = delete;
		virtual void _init() override;
		void _checkDepthFormat();
	};
} //namespace kgs

#endif // !VG_TEX_DEPTH_STENCIL_ATTACHMENT_H