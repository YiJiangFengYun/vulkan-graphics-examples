#ifndef VG_TEX_DEPTH_STENCIL_ATTACHMENT_H
#define VG_TEX_DEPTH_STENCIL_ATTACHMENT_H

#include "graphics/texture/texture.hpp"

namespace vg
{
	class TextureDepthStencilAttachment : public Texture
	{
	public:
		TextureDepthStencilAttachment(vk::Format format, uint32_t width, uint32_t height);
		~TextureDepthStencilAttachment();
		uint32_t getWidth() const;
		uint32_t getHeight() const;
	private:
		TextureDepthStencilAttachment() = delete;
		virtual void _init() override;
		void _checkDepthFormat();
	};
} //namespace kgs

#endif // !VG_TEX_DEPTH_STENCIL_ATTACHMENT_H