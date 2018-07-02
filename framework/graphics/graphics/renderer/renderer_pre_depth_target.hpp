#ifndef VG_RENDERER_PRE_DEPTH_TARGET_HPP
#define VG_RENDERER_PRE_DEPTH_TARGET_HPP

#include "graphics/global.hpp"
#include "graphics/texture/texture_2d.hpp"
#include "graphics/render_target/pre_depth_target.hpp"

namespace vg
{
    class RendererPreDepthTarget : public PreDepthTarget
    {
    public:
        RendererPreDepthTarget(uint32_t framebufferWidth = 0u
            , uint32_t framebufferHeight = 0u
            , vk::Format depthImageFormat = DEFAULT_DEPTH_FORMAT
            );
        const Texture2DDepthAttachment *getDepthTargetTexture() const;
    private:
        std::shared_ptr<Texture2DDepthAttachment> m_pDepthTargetTex;
        std::shared_ptr<vk::RenderPass> m_pMyRenderPass;
        std::shared_ptr<vk::Framebuffer> m_pMyFramebuffer;
        void _createObjs();
    };
} //vg

#endif //VG_RENDERER_PRE_DEPTH_TARGET_HPP