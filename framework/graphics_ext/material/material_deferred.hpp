#ifndef VGE_MATERIAL_DEFERRED_HPP
#define VGE_MATERIAL_DEFERRED_HPP

#include <graphics_ext/global.hpp>
#include <graphics_ext/cache/texture_cache.hpp>

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
            uint32_t deferredAttachmentCount;
            const DeferredAttachmentInfo *pDeferredAttachments;

            CreateInfo(uint32_t deferredAttachmentCount = 0u
                , const DeferredAttachmentInfo *pDeferredAttachments = nullptr);
        };

        MaterialDeferred(CreateInfo info = CreateInfo()
                , TextureCache<vg::Texture2DColorAttachment> *pColorAttachmentCache = nullptr
                , TextureCache<vg::Texture2DDepthStencilAttachment> *pDepthStencilAttachmentCache = nullptr);

        virtual void beginBindToRender(const BindInfo info, BindResult *pResult) override;
        virtual void endBindToRender(const EndBindInfo info) override;
    
        vg::Pass * getPassDeferred() const;
        vg::Pass * getPassComposition() const;

    private:
        TextureCache<vg::Texture2DColorAttachment> *m_pColorAttachmentCache;
        TextureCache<vg::Texture2DDepthStencilAttachment> *m_pDepthStencilAttachmentCache;

        std::shared_ptr<TextureCache<vg::Texture2DColorAttachment>> m_pMyColorAttachmentCache;
        std::shared_ptr<TextureCache<vg::Texture2DDepthStencilAttachment>> m_pMyDepthStencilAttachmentCache;

        vg::Texture2DColorAttachment *m_pColorAttachment;
        std::vector<vg::Texture2DColorAttachment *> m_pDeferredAttachments;
        vg::Texture2DDepthStencilAttachment *m_pDepthStencilAttachment;
        const vg::Texture::ImageView * m_pImageViewDepth;
        std::shared_ptr<vk::RenderPass> m_pRenderPass;
        std::shared_ptr<vk::Framebuffer> m_pFrameBuffer;
        std::shared_ptr<vg::Shader> m_pShaderDeferred;
        std::shared_ptr<vg::Shader> m_pShaderComposition;
        std::shared_ptr<vg::Pass> m_pPassDeferred;
        std::shared_ptr<vg::Pass> m_pPassComposition;

        std::unordered_map<vg::InstanceID, std::shared_ptr<vg::DimSepMesh2>> m_mapRectMeshes;

        void _createAttachments(CreateInfo createInfo);
        void _createRenderPass(CreateInfo createInfo);
        void _createFramebuffer(CreateInfo createInfo);
        void _createOtherPasses(CreateInfo createInfo);
        void _initPasses(CreateInfo createInfo);

        vg::DimSepMesh2 *_getRectMesh(vg::InstanceID id);
    };
} //vge

#endif //VGE_MATERIAL_DEFERRED_HPP