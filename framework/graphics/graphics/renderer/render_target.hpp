#ifndef VG_RENDER_TARGET_HPP
#define VG_RENDER_TARGET_HPP

#include "graphics/global.hpp"

namespace vg
{
    class BaseRenderTarget {
    public:
        BaseRenderTarget(uint32_t framebufferWidth = 0u
            , uint32_t framebufferHeight = 0u
            );
        uint32_t getFramebufferWidth() const;
        uint32_t getFramebufferHeight() const;
        const fd::Rect2D & getRenderArea() const;
        void setRenderArea(const fd::Rect2D & area);
        uint32_t getClearValueCount() const;
        const vk::ClearValue *getClearValues() const;
        void setClearValues(const vk::ClearValue *pClearValue, uint32_t clearValueCount);
    protected:
        uint32_t m_framebufferWidth;
        uint32_t m_framebufferHeight;
        fd::Rect2D m_renderArea;
        std::vector<vk::ClearValue> m_clearValues;
    };
    class RenderTarget : public BaseRenderTarget
    {
    public:
        static const vk::Format DEFAULT_DEPTH_STENCIL_FORMAT;

        RenderTarget(uint32_t framebufferWidth = 0u
            , uint32_t framebufferHeight = 0u
            , vk::Format colorImageFormat = vk::Format::eUndefined
            , vk::Format depthStencilImageFormat = DEFAULT_DEPTH_STENCIL_FORMAT
            );
        vk::Format getColorImageFormat() const;
        vk::Format getDepthStencilImageFormat() const;
        const vk::RenderPass * getFirstRenderPass() const;
        const vk::RenderPass * getSecondRenderPass() const;
        const vk::Framebuffer * getFirstFramebuffer() const;
        const vk::Framebuffer * getSecondFramebuffer() const;
    protected:
        
        vk::Format m_colorImageFormat;
        vk::Format m_depthStencilImageFormat;
        
        vk::RenderPass * m_pFirstRenderPass;
        vk::RenderPass * m_pSecondRenderPass;
        vk::Framebuffer * m_pFirstFramebuffer;
        vk::Framebuffer * m_pSecondFramebuffer;
        
    };
} //vg

#endif //VG_RENDER_TARGET_HPP