#ifndef VG_RENDERER_TARGET_HPP
#define VG_RENDERER_TARGET_HPP

#include "graphics/global.hpp"
#include "graphics/render_target/render_target.hpp"
#include "graphics/texture/attachment.hpp"

namespace vg
{
    class RendererTarget : public MultiRenderTarget
    {
    public:
        static const vk::Format DEFAULT_DEPTH_STENCIL_FORMAT;

        RendererTarget(uint32_t framebufferWidth = 0u
            , uint32_t framebufferHeight = 0u
            , vk::Format colorImageFormat = vk::Format::eUndefined
            , vk::Format depthStencilImageFormat = DEFAULT_DEPTH_STENCIL_FORMAT
            );
        vk::Format getColorImageFormat() const;
        vk::Format getDepthStencilImageFormat() const;
    protected:
        vk::Format m_colorImageFormat;
        vk::Format m_depthStencilImageFormat;
        BaseColorAttachment *m_pColorAttachment;
        BaseDepthStencilAttachment *m_pDepthStencilAttachment;
    };
} //vg

#endif //VG_RENDERER_TARGET_HPP