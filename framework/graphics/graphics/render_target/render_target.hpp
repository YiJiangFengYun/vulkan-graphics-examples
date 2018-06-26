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

    class OnceRenderTarget : public BaseRenderTarget
    {
    public:

        OnceRenderTarget(uint32_t framebufferWidth = 0u
            , uint32_t framebufferHeight = 0u
            );
        const vk::RenderPass * getRenderPass() const;
        const vk::Framebuffer * getFramebuffer() const;
    protected:
        vk::RenderPass *m_pRenderPass;
        vk::Framebuffer *m_pFramebuffer;
    };
    
    class MultiRenderTarget : public BaseRenderTarget
    {
    public:
        MultiRenderTarget(uint32_t framebufferWidth = 0u
            , uint32_t framebufferHeight = 0u
            );
        const vk::RenderPass * getFirstRenderPass() const;
        const vk::RenderPass * getSecondRenderPass() const;
        const vk::Framebuffer * getFirstFramebuffer() const;
        const vk::Framebuffer * getSecondFramebuffer() const;
    protected:
        vk::RenderPass *m_pFirstRenderPass;
        vk::RenderPass *m_pSecondRenderPass;
        vk::Framebuffer *m_pFirstFramebuffer;
        vk::Framebuffer *m_pSecondFramebuffer;
        // std::shared_ptr<vk::RenderPass> m_pFirstRenderPass;
        // std::shared_ptr<vk::RenderPass> m_pSecondRenderPass;
        // std::shared_ptr<vk::Framebuffer> m_pFirstFramebuffer;
        // std::shared_ptr<vk::Framebuffer> m_pSecondFramebuffer;
    };
} //vg

#endif //VG_RENDER_TARGET_HPP