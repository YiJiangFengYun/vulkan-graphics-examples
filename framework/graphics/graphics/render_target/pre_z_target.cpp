#include "graphics/render_target/pre_z_target.hpp"

namespace vg
{
    const vk::Format PreZTarget::DEFAULT_DEPTH_FORMAT(vk::Format::eD32Sfloat);

    PreZTarget::PreZTarget(uint32_t framebufferWidth
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

    vk::Format PreZTarget::getDepthImageFormat() const
    {
        return m_depthImageFormat;
    }

    const BaseDepthStencilAttachment *PreZTarget::getDepthAttachment() const
    {
        return m_pDepthAttachment.get();
    }
} //vg