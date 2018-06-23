#include "graphics/renderer/renderer_target.hpp"

namespace vg
{
    const vk::Format RendererTarget::DEFAULT_DEPTH_STENCIL_FORMAT(vk::Format::eD32SfloatS8Uint);

    RendererTarget::RendererTarget(uint32_t framebufferWidth
        , uint32_t framebufferHeight
        , vk::Format colorImageFormat
        , vk::Format depthStencilImageFormat
        )
        : MultiRenderTarget(framebufferWidth, framebufferHeight)
        , m_colorImageFormat(colorImageFormat)
        , m_depthStencilImageFormat(depthStencilImageFormat)
    {
        
    }

    vk::Format RendererTarget::getColorImageFormat() const
    {
        return m_colorImageFormat;
    }

    vk::Format RendererTarget::getDepthStencilImageFormat() const
    {
        return m_depthStencilImageFormat;
    }
} //vg