#ifndef VG_PRE_Z_TARGET_HPP
#define VG_PRE_Z_TARGET_HPP

#include "graphics/global.hpp"
#include "graphics/texture/texture_2d.hpp"
#include "graphics/renderer/render_target.hpp"

namespace vg
{
    class PreZTarget : public BaseRenderTarget
    {
    public:
        static const vk::Format DEFAULT_DEPTH_FORMAT;
        PreZTarget(uint32_t framebufferWidth = 0u
            , uint32_t framebufferHeight = 0u
            , vk::Format depthImageFormat = DEFAULT_DEPTH_FORMAT
            );
        
        vk::Format getDepthImageFormat() const;
        const Texture2DDepthAttachment *getDepthAttachment() const;
        const vk::RenderPass *getRenderPass() const;
        const vk::Framebuffer *getFramebuffer() const;
    private:
        vk::Format m_depthImageFormat;
        std::shared_ptr<Texture2DDepthAttachment> m_pDepthAttachment;
        std::shared_ptr<vk::RenderPass> m_pRenderPass;
        std::shared_ptr<vk::Framebuffer> m_pFramebuffer;
        void _createObjs();
    };
} //vg

#endif //VG_PRE_Z_TARGET_HPP