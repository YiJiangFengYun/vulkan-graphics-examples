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
        std::shared_ptr<BaseDepthStencilAttachment> m_pDepthStencilAttachment;
        std::shared_ptr<vk::RenderPass> m_pFirstColorTexRenderPass;        
        std::shared_ptr<vk::RenderPass> m_pSecondColorTexRenderPass;        
        std::shared_ptr<vk::Framebuffer> m_pFirstColorTexFramebuffer;
        std::shared_ptr<vk::Framebuffer> m_pSecondColorTexFramebuffer;
        void _createRenderPass();
        void _createDepthStencilTex();
        void _createFramebuffer();
    };
} //vg

#endif //VG_RENDER_TARGET_COLOR_TEXTURE_HPP