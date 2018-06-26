#ifndef VG_RENDERER_POST_RENDER_TARGET_HPP
#define VG_RENDERER_POST_RENDER_TARGET_HPP

#include "graphics/global.hpp"
#include "graphics/texture/texture_2d.hpp"
#include "graphics/texture/texture_depth_stencil_attachment.hpp"
#include "graphics/render_target/post_render_target.hpp"

namespace vg
{
    class RendererPostRenderTarget : public PostRenderTarget
    {
    public:
        RendererPostRenderTarget(uint32_t framebufferWidth = 0u
            , uint32_t framebufferHeight = 0u
            , vk::Format colorImageFormat = DEFAULT_COLOR_FORMAT
            , vk::Format depthStencilImageFormat = PostRenderTarget::DEFAULT_DEPTH_STENCIL_FORMAT
            );
    private:
        std::shared_ptr<Texture2DColorAttachment> m_pMyColorAttachment;
        std::shared_ptr<TextureDepthStencilAttachment> m_pMyDepthStencilAttachment;
        std::shared_ptr<vk::RenderPass> m_pMyRenderPass;
        std::shared_ptr<vk::Framebuffer> m_pMyFramebuffer;
        void _createObjs();
    };
} //vg

#endif //VG_RENDERER_POST_RENDER_TARGET_HPP