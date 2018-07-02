#include "graphics/render_target/pre_depth_target.hpp"

namespace vg
{
    const vk::Format PreDepthTarget::DEFAULT_DEPTH_FORMAT(vk::Format::eD32Sfloat);

    PreDepthTarget::PreDepthTarget(uint32_t framebufferWidth
        , uint32_t framebufferHeight
        , vk::Format depthImageFormat
        )
        : OnceRenderTarget(framebufferWidth, framebufferHeight)
        , m_depthImageFormat(depthImageFormat)
        , m_pDepthAttachment()
    {
        vk::ClearValue clearValueDepthStencil = {
            vk::ClearDepthStencilValue(1.0f, 0)
        };
        setClearValues(&clearValueDepthStencil, 1u);
    }

    vk::Format PreDepthTarget::getDepthImageFormat() const
    {
        return m_depthImageFormat;
    }

    const vk::ImageView *PreDepthTarget::getDepthAttachment() const
    {
        return m_pDepthAttachment;
    }
} //vg