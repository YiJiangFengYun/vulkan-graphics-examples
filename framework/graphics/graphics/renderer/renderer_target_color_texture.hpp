#ifndef VG_RENDERER_TARGET_COLOR_TEXTURE_HPP
#define VG_RENDERER_TARGET_COLOR_TEXTURE_HPP

#include "graphics/renderer/renderer_target.hpp"
#include "graphics/texture/texture_depth_stencil_attachment.hpp"

namespace vg
{
    class ColorTexRendererTarget : public RendererTarget
    {
    public:
        ColorTexRendererTarget(BaseColorAttachment *pColorAttachmentTex);
    private:
        //Renderer will render to color texture when it is not null.
        BaseColorAttachment *m_pColorAttchment;
        std::shared_ptr<TextureDepthStencilAttachment> m_pMyDepthStencilAttachment;
        std::shared_ptr<vk::RenderPass> m_pMyFirstRenderPass;
        std::shared_ptr<vk::RenderPass> m_pMySecondRenderPass;
        std::shared_ptr<vk::Framebuffer> m_pMyFirstFramebuffer;
        std::shared_ptr<vk::Framebuffer> m_pMySecondFramebuffer;
        void _createRenderPass();
        void _createDepthStencilTex();
        void _createFramebuffer();
    };
} //vg

#endif //VG_RENDERER_TARGET_COLOR_TEXTURE_HPP