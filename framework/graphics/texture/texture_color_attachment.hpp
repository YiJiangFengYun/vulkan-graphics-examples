#ifndef VG_TEX_COLOR_ATTACHMENT_H
#define VG_TEX_COLOR_ATTACHMENT_H

#include "graphics/texture/texture.hpp"

namespace vg
{
	class TextureColorAttachment : public Texture
	{
	public:
		TextureColorAttachment(TextureFormat format, uint32_t width, uint32_t height);
		~TextureColorAttachment();
		uint32_t getWidth() const;
		uint32_t getHeight() const;
	private:
		TextureColorAttachment() = delete;
		inline void _init() override
		{
			Texture::_init();
			//Transform Image layout to final layout.
			auto pCommandBuffer = beginSingleTimeCommands();
			_tranImageLayout(pCommandBuffer, *m_pImage, m_currVkImageLayout, m_vkImageLayout,
				0, m_mipMapLevels, 0, m_arrayLayer);
			endSingleTimeCommands(pCommandBuffer);
			m_currVkImageLayout = m_vkImageLayout;
		}
	};
} //namespace kgs

#endif // !VG_TEX_COLOR_ATTACHMENT_H
