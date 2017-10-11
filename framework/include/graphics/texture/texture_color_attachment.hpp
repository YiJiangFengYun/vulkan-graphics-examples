#ifndef KGS_TEX_COLOR_ATTACHMENT_H
#define KGS_TEX_COLOR_ATTACHMENT_H

#include "graphics/texture/texture.hpp"

namespace kgs
{
	class TextureColorAttachment : public Texture
	{
	public:
		TextureColorAttachment(TextureFormat format, uint32_t width, uint32_t height);
		~TextureColorAttachment();
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

#endif // !KGS_TEX_COLOR_ATTACHMENT_H
