#ifndef VG_RENDER_TARGET_COLOR_TEXTURE_HPP
#define VG_RENDER_TARGET_COLOR_TEXTURE_HPP

#include "graphics/renderer/render_target.hpp"
#include "graphics/texture/attachment.hpp"

namespace vg
{
    class ColorTexRenderTarget : public RenderTarget
	{
	public:
	    ColorTexRenderTarget(BaseColorAttachment *pColorAttachmentTex);
	private:
	    //Renderer will render to color texture when it is not null.
		BaseColorAttachment *m_pColorAttchment;
		std::shared_ptr<vk::RenderPass> m_pColorTexRenderPass;
		std::shared_ptr<BaseDepthStencilAttachment> m_pDepthStencilAttachment;
		std::shared_ptr<vk::Framebuffer> m_pColorTexFramebuffer;
		void _createRenderPass();
		void _createDepthStencilTex();
		void _createFramebuffer();
	};
} //vg

#endif //VG_RENDER_TARGET_COLOR_TEXTURE_HPP