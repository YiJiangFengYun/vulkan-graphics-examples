#ifndef VG_LIGHT_DEPTH_TARGET_CUBE_HPP
#define VG_LIGHT_DEPTH_TARGET_CUBE_HPP

#include "graphics/global.hpp"
#include "graphics/render_target/pre_z_target.hpp"
#include "graphics/texture/texture_cube.hpp"

namespace vg
{
    class LightDepthCubeFaceTarget : public PreZTarget
    {
    public:
        LightDepthCubeFaceTarget(uint32_t framebufferWidth = 0u
            , uint32_t framebufferHeight = 0u
            , vk::Format depthImageFormat = DEFAULT_DEPTH_FORMAT
            , const vk::ImageView *pFaceImageView = nullptr
            );
        
    private:
        const vk::ImageView *m_pFaceImageView;
        std::shared_ptr<vk::RenderPass> m_pMyRenderPass;
        std::shared_ptr<vk::Framebuffer> m_pMyFramebuffer;

        void _createObjs();
    };

    class LightDepthCubeTargets
    {
    public:
        using PFaceTargetArray = std::array<std::shared_ptr<LightDepthCubeFaceTarget>, static_cast<size_t>(CubemapFace::RANGE_SIZE)>;
        static const vk::Format DEFAULT_DEPTH_FORMAT;
        LightDepthCubeTargets(uint32_t framebufferWidth = 0u
            , uint32_t framebufferHeight = 0u
            , vk::Format depthImageFormat = DEFAULT_DEPTH_FORMAT
        );

        vk::Format getDepthImageFormat() const;
        const TextureCubeDepthAttachment *getDepthTargetTexture() const;
        const PFaceTargetArray &getFaceTargets() const;
    private:
        uint32_t m_framebufferWidth;
        uint32_t m_framebufferHeight;
        vk::Format m_depthImageFormat;
        std::shared_ptr<TextureCubeDepthAttachment> m_pDepthTargetTex;
        PFaceTargetArray m_pFaceTargets;
        void _createObjs();
    };
} //vg

#endif //VG_LIGHT_DEPTH_TARGET_CUBE_HPP