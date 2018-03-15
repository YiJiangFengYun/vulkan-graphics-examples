#ifndef VG_TEX_COLOR_ATTACHMENT_H
#define VG_TEX_COLOR_ATTACHMENT_H

#include "graphics/texture/texture.hpp"

namespace vg
{
	class TextureColorAttachment : public Texture
	{
	public:
		TextureColorAttachment(vk::Format format, uint32_t width, uint32_t height);
		~TextureColorAttachment();
		uint32_t getWidth() const;
		uint32_t getHeight() const;
	private:
		TextureColorAttachment() = delete;
		virtual void _init() override;
	};
} //namespace kgs

#endif // !VG_TEX_COLOR_ATTACHMENT_H
