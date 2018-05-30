#ifndef VG_RENDER_TARGET_SURFACE_HPP
#define VG_RENDER_TARGET_SURFACE_HPP

#include "graphics/texture/attachment.hpp"
#include "graphics/renderer/render_target.hpp"

namespace vg
{
    class SurfaceRenderTarget : public RenderTarget 
    {
    public:
        SurfaceRenderTarget(uint32_t swapchainImageViewCount
            , vk::ImageView *pSwapchainImageViews
            , vk::Format swapchainImageFormat
            , uint32_t swapchainImageWidth
            , uint32_t swapchainImageHeight);
        void setImageIndex(uint32_t imageIndex);
    private:
        std::shared_ptr<BaseDepthStencilAttachment> m_pDepthStencilAttachment;    
        std::shared_ptr<vk::RenderPass> m_pFirstSurfaceRenderPass;
        std::shared_ptr<vk::RenderPass> m_pSecondSurfaceRenderPass;
        std::vector<std::shared_ptr<vk::Framebuffer>> m_pFirstFramebuffers;        
        std::vector<std::shared_ptr<vk::Framebuffer>> m_pSecondFramebuffers;
        //Renderer will use swapchain image when color attachment texture is null.
        uint32_t m_swapchainImageViewCount;
        vk::ImageView *m_pSwapchainImageViews;
        uint32_t m_imageIndex;

        void _createRenderPass();
        void _createDepthStencilTex();
        void _createFramebuffers();
    };
} //vg

#endif //VG_RENDER_TARGET_SURFACE_HPP