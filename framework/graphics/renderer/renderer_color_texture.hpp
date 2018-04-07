#ifndef VG_RENDERER_COLOR_TEXTURE_HPP
#define VG_RENDERER_COLOR_TEXTURE_HPP

#include "graphics/renderer/renderer.hpp"

namespace vg
{
    class ColorTexRenderer : public Renderer
	{
	public:
	    ColorTexRenderer(BaseColorAttachment *pColorAttachmentTex);
	private:
	    //Renderer will render to color texture when it is not null.
		BaseColorAttachment *m_pColorAttchment;
		std::shared_ptr<vk::RenderPass> m_pRenderPass;
		std::shared_ptr<BaseDepthStencilAttachment> m_pDepthStencilAttachment;
		std::shared_ptr<vk::Framebuffer> m_pFrameBuffer;
		void _createRenderPass();
		void _createDepthStencilTex();
		void _createFramebuffer();
	};
} //vg

#endif //VG_RENDERER_COLOR_TEXTURE_HPP