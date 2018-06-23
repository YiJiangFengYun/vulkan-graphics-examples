#ifndef VG_PRE_Z_TARGET_HPP
#define VG_PRE_Z_TARGET_HPP

#include "graphics/global.hpp"
#include "graphics/texture/texture_2d.hpp"
#include "graphics/render_target/render_target.hpp"

namespace vg
{
    class PreZTarget : public OnceRenderTarget
    {
    public:
        static const vk::Format DEFAULT_DEPTH_FORMAT;
        PreZTarget(uint32_t framebufferWidth = 0u
            , uint32_t framebufferHeight = 0u
            , vk::Format depthImageFormat = DEFAULT_DEPTH_FORMAT
            );
        
        vk::Format getDepthImageFormat() const;
        const Texture2DDepthAttachment *getDepthAttachment() const;
    private:
        vk::Format m_depthImageFormat;
        std::shared_ptr<Texture2DDepthAttachment> m_pDepthAttachment;
        void _createObjs();
    };
} //vg

#endif //VG_PRE_Z_TARGET_HPP