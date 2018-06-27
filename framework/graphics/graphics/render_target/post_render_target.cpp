#include "graphics/render_target/post_render_target.hpp"

namespace vg
{
    const vk::Format PostRenderTarget::DEFAULT_COLOR_FORMAT(vk::Format::eR8G8B8A8Unorm);
    const vk::Format PostRenderTarget::DEFAULT_DEPTH_STENCIL_FORMAT(vk::Format::eD32SfloatS8Uint);

    PostRenderTarget::PostRenderTarget(uint32_t framebufferWidth
        , uint32_t framebufferHeight
        , vk::Format colorImageFormat
        , vk::Format depthStencilImageFormat
        )
        : OnceRenderTarget(framebufferWidth, framebufferHeight)
        , m_colorImageFormat(colorImageFormat)
        , m_depthStencilImageFormat(depthStencilImageFormat)
        , m_pColorAttachment()
        , m_pDepthStencilAttachment()
    {
        vk::ClearValue clearValueColor = {
            std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f}
        };
        vk::ClearValue clearValueDepthStencil = {
            vk::ClearDepthStencilValue(1.0f, 0)
        };
        std::array<vk::ClearValue, 2> clearValues = { clearValueColor
            , clearValueDepthStencil
        };
        setClearValues(clearValues.data(), static_cast<uint32_t>(clearValues.size()));
    }
        
    vk::Format PostRenderTarget::getColorImageFormat() const
    {
        return m_colorImageFormat;
    }

    vk::Format PostRenderTarget::getDepthStencilImageFormat() const
    {
        return m_depthStencilImageFormat;
    }
        
    const vk::ImageView *PostRenderTarget::getColorAttachment() const
    {
        return m_pColorAttachment;
    }

    const vk::ImageView *PostRenderTarget::getDepthStencilAttachment() const
    {
        return m_pDepthStencilAttachment;
    }
} //vg