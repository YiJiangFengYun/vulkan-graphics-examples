#ifndef VG_POST_RENDER_TARGET_HPP
#define VG_POST_RENDER_TARGET_HPP

#include "graphics/global.hpp"
#include "graphics/texture/attachment.hpp"
#include "graphics/render_target/render_target.hpp"

namespace vg
{
    class PostRenderTarget : public OnceRenderTarget
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
        const BaseColorAttachment *getColorAttachment() const;
        const BaseDepthStencilAttachment *getDepthStencilAttachment() const;
    protected:
        vk::Format m_colorImageFormat;
        vk::Format m_depthStencilImageFormat;
        BaseColorAttachment *m_pColorAttachment;
        BaseDepthStencilAttachment *m_pDepthStencilAttachment;
    };
} //vg

#endif //VG_POST_RENDER_TARGET_HPP