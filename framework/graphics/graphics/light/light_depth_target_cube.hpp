#ifndef VG_LIGHT_DEPTH_TARGET_CUBE_HPP
#define VG_LIGHT_DEPTH_TARGET_CUBE_HPP

#include "graphics/global.hpp"
#include "graphics/render_target/pre_depth_target.hpp"
#include "graphics/texture/texture_cube.hpp"
#include "graphics/texture/texture_depth_stencil_attachment.hpp"

namespace vg
{
    class LightDepthCubeFaceTarget : public OnceRenderTarget
    {
    public:
        static const vk::Format DEFAULT_COLOR_FORMAT;
        static const vk::Format DEFAULT_DEPTH_FORMAT;
        LightDepthCubeFaceTarget(uint32_t framebufferWidth = 0u
            , uint32_t framebufferHeight = 0u
            , vk::Format colorImageFormat = DEFAULT_COLOR_FORMAT
            , vk::Format depthImageFormat = DEFAULT_DEPTH_FORMAT
            , const vk::ImageView *pFaceColorImageView = nullptr
            , const vk::ImageView *pFaceDepthImageView = nullptr
            );
        vk::Format getColorImageFormat() const;
        vk::Format getDepthStencilImageFormat() const;
        const vk::ImageView *getColorAttachment() const;
        const vk::ImageView *getDepthAttachment() const;
    private:
        vk::Format m_colorImageFormat;
        vk::Format m_depthImageFormat;
        const vk::ImageView *m_pFaceColorImageView;
        const vk::ImageView *m_pFaceDepthImageView;
        std::shared_ptr<vk::RenderPass> m_pMyRenderPass;
        std::shared_ptr<vk::Framebuffer> m_pMyFramebuffer;

        void _createObjs();
    };

    class LightDepthCubeTargets
    {
    public:
        using PFaceTargetArray = std::array<std::shared_ptr<LightDepthCubeFaceTarget>, static_cast<size_t>(CubemapFace::RANGE_SIZE)>;
        static const vk::Format DEFAULT_COLOR_FORMAT;
        static const vk::Format DEFAULT_DEPTH_FORMAT;
        LightDepthCubeTargets(uint32_t framebufferWidth = 0u
            , uint32_t framebufferHeight = 0u
            , vk::Format colorImageFormat = DEFAULT_COLOR_FORMAT
            , vk::Format depthImageFormat = DEFAULT_DEPTH_FORMAT
        );
        vk::Format getColorImageFormat() const;
        vk::Format getDepthImageFormat() const;
        const TextureCubeColorAttachment *getColorTargetTexture() const;
        const TextureDepthAttachment *getDepthTargetTexture() const;
        const std::array<const LightDepthCubeFaceTarget *, static_cast<size_t>(CubemapFace::RANGE_SIZE)> &getFaceTargets() const;
    private:
        uint32_t m_framebufferWidth;
        uint32_t m_framebufferHeight;
        vk::Format m_colorImageFormat;
        vk::Format m_depthImageFormat;
        std::shared_ptr<TextureCubeColorAttachment> m_pColorTargetTex;
        std::shared_ptr<TextureDepthAttachment> m_pDepthTargetTex;
        PFaceTargetArray m_pFaceTargets;
        std::array<const LightDepthCubeFaceTarget *, static_cast<size_t>(CubemapFace::RANGE_SIZE)> m_refFaceTargets;
        void _createObjs();
    };

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