#ifndef KGS_TEX_DEPTH_STENCIL_ATTACHMENT_H
#define KGS_TEX_DEPTH_STENCIL_ATTACHMENT_H

#include "graphics/texture/texture.hpp"

namespace kgs
{
	class TextureDepthStencilAttachment : public Texture
	{
	public:
		TextureDepthStencilAttachment(TextureFormat format, uint32_t width, uint32_t height);
		~TextureDepthStencilAttachment();
	private:
		TextureDepthStencilAttachment() = delete;
		inline void _init() override
		{
			_checkDepthFormat();
			Texture::_init();
			//Transform Image layout to final layout.
			auto pCommandBuffer = beginSingleTimeCommands();
			_tranImageLayout(pCommandBuffer, *m_pImage, m_currVkImageLayout, m_vkImageLayout,
				0, m_mipMapLevels, 0, m_arrayLayer);
			endSingleTimeCommands(pCommandBuffer);
			m_currVkImageLayout = m_vkImageLayout;
		}

		inline void _checkDepthFormat()
		{
			_updateVkFormat();
			auto pPhysicalDevice = pContext->getPhysicalDevice();
			auto props = pPhysicalDevice->getFormatProperties(m_vkFormat);
			if ((props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment) !=
				vk::FormatFeatureFlagBits::eDepthStencilAttachment)
			{
				throw std::runtime_error("Depth format is not supported!");
			}
		}
	};
} //namespace kgs

#endif // !KGS_TEX_DEPTH_STENCIL_ATTACHMENT_H