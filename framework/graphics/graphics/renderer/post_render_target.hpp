#ifndef VG_POST_RENDER_TARGET_HPP
#define VG_POST_RENDER_TARGET_HPP

#include "graphics/global.hpp"
#include "graphics/texture/texture_2d.hpp"
#include "graphics/texture/texture_depth_stencil_attachment.hpp"
#include "graphics/renderer/render_target.hpp"

namespace vg
{
    class PostRenderTarget : public BaseRenderTarget
    {
    public:
        static const vk::Format DEFAULT_COLOR_FORMAT;
        static const vk::Format DEFAULT_DEPTH_STENCIL_FORMAT;
        PostRenderTarget(uint32_t framebufferWidth = 0u
            , uint32_t framebufferHeight = 0u
            , vk::Format colorImageFormat = DEFAULT_COLOR_FORMAT
            , vk::Format depthStencilImageFormat = DEFAULT_DEPTH_STENCIL_FORMAT
            );
        vk::Format getColorImageFormat() const;
        vk::Format getDepthStencilImageFormat() const;
        const Texture2DColorAttachment *getColorAttachment() const;
        const TextureDepthStencilAttachment *getDepthStencilAttachment() const;
        const vk::RenderPass *getRenderPass() const;
        const vk::Framebuffer *getFramebuffer() const;
    private:
        vk::Format m_colorImageFormat;
        vk::Format m_depthStencilImageFormat;
        std::shared_ptr<Texture2DColorAttachment> m_pColorAttachment;
        std::shared_ptr<TextureDepthStencilAttachment> m_pDepthStencilAttachment;
        std::shared_ptr<vk::RenderPass> m_pRenderPass;
        std::shared_ptr<vk::Framebuffer> m_pFramebuffer;

        void _createObjs();
    };
} //vg

#endif //VG_POST_RENDER_TARGET_HPP