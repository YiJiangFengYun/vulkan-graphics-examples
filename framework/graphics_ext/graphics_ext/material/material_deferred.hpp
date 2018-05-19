#ifndef VGE_MATERIAL_DEFERRED_HPP
#define VGE_MATERIAL_DEFERRED_HPP

#include "graphics_ext/global.hpp"

namespace vge
{
    class MaterialDeferred : public vg::Material
    {
    public:
        struct DeferredAttachmentInfo {
            vk::Format format;

            DeferredAttachmentInfo(vk::Format format = vk::Format::eUndefined);
        };
        struct CreateInfo 
        {
            uint32_t framebufferWidth;
            uint32_t framebufferHeight;
            vk::Format colorAttachmentFormat;
            vk::Format depthStencilAttachmentFormat;
            uint32_t deferredAttachmentCount;
            const DeferredAttachmentInfo *pDeferredAttachments;
            CreateInfo(uint32_t framebufferWidth = 0u
                , uint32_t framebufferHeight = 0u
                , vk::Format colorAttachmentFormat = vk::Format::eUndefined
                , vk::Format depthStencilAttachmentFormat = vk::Format::eUndefined
                , uint32_t deferredAttachmentCount = 0u
                , const DeferredAttachmentInfo *pDeferredAttachments = nullptr);
        };

        MaterialDeferred(CreateInfo info = CreateInfo());
    
        vg::Pass * getPassDeferred() const;
        vg::Pass * getPassComposition() const;

    private:
        CreateInfo m_info;
        std::vector<vge::MaterialDeferred::DeferredAttachmentInfo> m_deferredAttachmentInfos;

        std::shared_ptr<vg::Texture2DColorAttachment> m_pColorAttachment;
        std::shared_ptr<vg::Texture2DDepthStencilAttachment> m_pDepthStencilAttachment;
        std::vector<std::shared_ptr<vg::Texture2DColorAttachment>> m_arrPDeferredAttachments;

        std::shared_ptr<vk::RenderPass> m_pRenderPass;
        std::shared_ptr<vk::Framebuffer> m_pFrameBuffer;
        std::shared_ptr<vg::Shader> m_pShaderDeferred;
        std::shared_ptr<vg::Shader> m_pShaderComposition;
        std::shared_ptr<vg::Pass> m_pPassDeferred;
        std::shared_ptr<vg::Pass> m_pPassComposition;

        std::shared_ptr<vg::DimSepMesh2> m_pRectMesh;

        virtual void _beginBind(const BindInfo info, BindResult *pResult) const override;

        void _createDeferredAttachments(CreateInfo createInfo);
        void _createRenderPass(CreateInfo createInfo);
        void _createFramebuffer();
        void _createOtherPasses(CreateInfo createInfo);
        void _initPasses(CreateInfo createInfo);              

         vg::DimSepMesh2 *_getRectMesh() const;

    };
} //vge

#endif //VGE_MATERIAL_DEFERRED_HPP