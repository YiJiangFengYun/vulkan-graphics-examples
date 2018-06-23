#ifndef VG_RENDERER_TARGET_COLOR_TEXTURE_HPP
#define VG_RENDERER_TARGET_COLOR_TEXTURE_HPP

#include "graphics/renderer/renderer_target.hpp"
#include "graphics/texture/attachment.hpp"

namespace vg
{
    class ColorTexRendererTarget : public RendererTarget
    {
    public:
        ColorTexRendererTarget(BaseColorAttachment *pColorAttachmentTex);
    private:
        //Renderer will render to color texture when it is not null.
        BaseColorAttachment *m_pColorAttchment;
        std::shared_ptr<BaseDepthStencilAttachment> m_pDepthStencilAttachment;
        void _createRenderPass();
        void _createDepthStencilTex();
        void _createFramebuffer();
    };
} //vg

#endif //VG_RENDERER_TARGET_COLOR_TEXTURE_HPP