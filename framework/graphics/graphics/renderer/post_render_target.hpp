#ifndef VG_POST_RENDER_TARGET_HPP
#define VG_POST_RENDER_TARGET_HPP

#include "graphics/global.hpp"
#include "graphics/texture/texture_2d.hpp"
#include "graphics/texture/texture_depth_stencil_attachment.hpp"

namespace vg
{
    class PostRenderTarget
    {
    public:
        static const vk::Format DEFAULT_COLOR_FORMAT;
		static const vk::Format DEFAULT_DEPTH_STENCIL_FORMAT;
        PostRenderTarget(uint32_t framebufferWidth = 0u
            , uint32_t framebufferHeight = 0u
            , vk::Format colorImageFormat = DEFAULT_COLOR_FORMAT
            , vk::Format depthStencilImageFormat = DEFAULT_DEPTH_STENCIL_FORMAT
            );
        uint32_t getFramebufferWidth() const;
        uint32_t getFramebufferHeight() const;
        vk::Format getColorImageFormat() const;
        vk::Format getDepthStencilImageFormat() const;
        fd::Rect2D getRenderArea() const;
        const Texture2DColorAttachment *getColorAttachment() const;
        const TextureDepthStencilAttachment *getDepthStencilAttachment() const;
        const vk::RenderPass *getRenderPass() const;
        const vk::Framebuffer *getFramebuffer() const;
    private:
        uint32_t m_framebufferWidth;
		uint32_t m_framebufferHeight;
        vk::Format m_colorImageFormat;
		vk::Format m_depthStencilImageFormat;
        fd::Rect2D m_renderArea;
		std::shared_ptr<Texture2DColorAttachment> m_pColorAttachment;
		std::shared_ptr<TextureDepthStencilAttachment> m_pDepthStencilAttachment;
		std::shared_ptr<vk::RenderPass> m_pRenderPass;
		std::shared_ptr<vk::Framebuffer> m_pFramebuffer;

        void _createObjs();
    };
} //vg

#endif //VG_POST_RENDER_TARGET_HPP