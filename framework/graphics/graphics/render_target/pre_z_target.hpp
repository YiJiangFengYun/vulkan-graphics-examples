#ifndef VG_PRE_Z_TARGET_HPP
#define VG_PRE_Z_TARGET_HPP

#include "graphics/global.hpp"
#include "graphics/texture/attachment.hpp"
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
        const BaseDepthStencilAttachment *getDepthAttachment() const;
    protected:
        vk::Format m_depthImageFormat;
        std::shared_ptr<BaseDepthStencilAttachment> m_pDepthAttachment;
    };
} //vg

#endif //VG_PRE_Z_TARGET_HPP