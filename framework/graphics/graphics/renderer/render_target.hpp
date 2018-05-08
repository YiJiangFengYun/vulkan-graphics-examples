#ifndef VG_RENDER_TARGET_HPP
#define VG_RENDER_TARGET_HPP

#include "graphics/global.hpp"

namespace vg
{
    class RenderTarget
    {
    public:
        static const vk::Format DEFAULT_DEPTH_STENCIL_FORMAT;

        RenderTarget();
        uint32_t getFramebufferWidth() const;
        uint32_t getFramebufferHeight() const;
        vk::Format getColorImageFormat() const;
        vk::Format getDepthStencilImageFormat() const;
        const fd::Rect2D & getRenderArea() const;
        void setRenderArea(const fd::Rect2D & area);
        const vk::RenderPass * getFirstRenderPass() const;
        const vk::RenderPass * getSecondRenderPass() const;
        const vk::Framebuffer * getFirstFramebuffer() const;
        const vk::Framebuffer * getSecondFramebuffer() const;
    protected:
        uint32_t m_framebufferWidth;
		uint32_t m_framebufferHeight;
		vk::Format m_colorImageFormat;
		vk::Format m_depthStencilImageFormat;
        fd::Rect2D m_renderArea;
		vk::RenderPass * m_pFirstRenderPass;
        vk::RenderPass * m_pSecondRenderPass;
		vk::Framebuffer * m_pFirstFramebuffer;
        vk::Framebuffer * m_pSecondFramebuffer;
    };
} //vg

#endif //VG_RENDER_TARGET_HPP