#include "graphics/renderer/render_target.hpp"

namespace vg
{
    const vk::Format RenderTarget::DEFAULT_DEPTH_STENCIL_FORMAT(vk::Format::eD32SfloatS8Uint);

    BaseRenderTarget::BaseRenderTarget(uint32_t framebufferWidth
        , uint32_t framebufferHeight
        )
        : m_framebufferWidth(framebufferWidth)
        , m_framebufferHeight(framebufferHeight)
        , m_renderArea(0.0f, 0.0f, 1.0f, 1.0f)
        , m_clearValues()
    {

    }

    uint32_t BaseRenderTarget::getFramebufferWidth() const
    {
        return m_framebufferWidth;
    }

    uint32_t BaseRenderTarget::getFramebufferHeight() const
    {
        return m_framebufferHeight;
    }

    const fd::Rect2D & BaseRenderTarget::getRenderArea() const
    {
        return m_renderArea;
    }

    void BaseRenderTarget::setRenderArea(const fd::Rect2D & area)
    {
#ifdef DEBUG
        if (area.width < 0)
            throw std::invalid_argument("The width of area is smaller than 0!");
        else if (area.width > 1)
            throw std::invalid_argument("The width of area is bigger than 1!");
        if (area.height < 0)
            throw std::invalid_argument("The height of area is smaller than 0!");
        else if (area.height > 1)
            throw std::invalid_argument("The height of area is bigger than 1!");
        if (area.x < 0)
            throw std::invalid_argument("the x of area is smaller than 0!");
        else if (area.x > area.width)
            throw std::invalid_argument("The x of area is bigger than the width of area!");
        if (area.y < 0)
            throw std::invalid_argument("the y of area is smaller than 0!");
        else if (area.y > area.height)
            throw std::invalid_argument("The y of area is bigger than the height of area!");
#endif // DEBUG

        m_renderArea = area;
    }

    uint32_t BaseRenderTarget::getClearValueCount() const
    {
        return static_cast<uint32_t>(m_clearValues.size());
    }

    const vk::ClearValue *BaseRenderTarget::getClearValues() const
    {
        return m_clearValues.data();
    }

    void BaseRenderTarget::setClearValues(const vk::ClearValue *pClearValues, uint32_t clearValueCount)
    {
        m_clearValues.resize(clearValueCount);
        memcpy(m_clearValues.data(), pClearValues, sizeof(vk::ClearValue) * clearValueCount);
    }

    RenderTarget::RenderTarget(uint32_t framebufferWidth
        , uint32_t framebufferHeight
        , vk::Format colorImageFormat
        , vk::Format depthStencilImageFormat
        )
        : BaseRenderTarget(framebufferWidth, framebufferHeight)
        , m_colorImageFormat(colorImageFormat)
        , m_depthStencilImageFormat(depthStencilImageFormat)
        , m_pFirstRenderPass()
        , m_pSecondRenderPass()
        , m_pFirstFramebuffer()
        , m_pSecondFramebuffer()
        
    {
        
    }

    vk::Format RenderTarget::getColorImageFormat() const
    {
        return m_colorImageFormat;
    }

    vk::Format RenderTarget::getDepthStencilImageFormat() const
    {
        return m_depthStencilImageFormat;
    }

    const vk::RenderPass *RenderTarget::getFirstRenderPass() const
    {
        return m_pFirstRenderPass;
    }

    const vk::RenderPass *RenderTarget::getSecondRenderPass() const
    {
        return m_pSecondRenderPass;
    }

    const vk::Framebuffer *RenderTarget::getFirstFramebuffer() const
    {
        return m_pFirstFramebuffer;
    }

    const vk::Framebuffer *RenderTarget::getSecondFramebuffer() const
    {
        return m_pSecondFramebuffer;
    }

} //vg