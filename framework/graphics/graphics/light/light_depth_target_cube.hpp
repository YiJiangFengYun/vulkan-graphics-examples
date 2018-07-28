#ifndef VG_LIGHT_DEPTH_TARGET_CUBE_HPP
#define VG_LIGHT_DEPTH_TARGET_CUBE_HPP

#include "graphics/global.hpp"
#include "graphics/render_target/pre_depth_target.hpp"
#include "graphics/texture/texture_cube.hpp"
#include "graphics/texture/texture_depth_stencil_attachment.hpp"

namespace vg
{
    class LightDistTargetCube : public OnceRenderTarget
    {
    public:
        static const vk::Format DEFAULT_COLOR_FORMAT;
        static const vk::Format DEFAULT_DEPTH_FORMAT;
        LightDistTargetCube(uint32_t framebufferWidth = 0u
            , uint32_t framebufferHeight = 0u
            , vk::Format colorImageFormat = DEFAULT_COLOR_FORMAT
            , vk::Format depthImageFormat = DEFAULT_DEPTH_FORMAT
            );
        vk::Format getColorImageFormat() const;
        vk::Format getDepthImageFormat() const;
        const TextureCubeColorAttachment *getColorTargetTexture() const;
        const TextureDepthAttachment *getDepthTargetTexture() const;
    private:
        vk::Format m_colorImageFormat;
        vk::Format m_depthImageFormat;
        std::shared_ptr<TextureCubeColorAttachment> m_pColorTargetTex;
        std::shared_ptr<TextureDepthAttachment> m_pDepthTargetTex;
        std::shared_ptr<vk::RenderPass> m_pMyRenderPass;
        std::shared_ptr<vk::Framebuffer> m_pMyFramebuffer;
        void _createObjs();
    };
} //vg

#endif //VG_LIGHT_DEPTH_TARGET_CUBE_HPP