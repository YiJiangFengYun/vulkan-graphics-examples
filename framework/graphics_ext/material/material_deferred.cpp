#include "graphics_ext/material/material_deferred.hpp"

namespace vge
{
    MaterialDeferred::DeferredAttachmentInfo::DeferredAttachmentInfo(vk::Format format)
        : format(format)
    {}

    MaterialDeferred::CreateInfo::CreateInfo(uint32_t deferredAttachmentCount
        , const DeferredAttachmentInfo *pDeferredAttachments
        )
        : deferredAttachmentCount(deferredAttachmentCount)
        , pDeferredAttachments(pDeferredAttachments)
    {}

    MaterialDeferred::MaterialDeferred(CreateInfo info
        , TextureCache<vg::Texture2DColorAttachment> *pColorAttachmentCache
        , TextureCache<vg::Texture2DDepthStencilAttachment> *pDepthStencilAttachmentCache
        )
        : vg::Material()
        , m_pColorAttachmentCache(pColorAttachmentCache)
        , m_pDepthStencilAttachmentCache(pDepthStencilAttachmentCache)
        , m_pMyColorAttachmentCache()
        , m_pMyDepthStencilAttachmentCache()
    {
        if (pColorAttachmentCache == nullptr) {
            m_pMyColorAttachmentCache = std::shared_ptr<TextureCache<vg::Texture2DColorAttachment>> {
                new TextureCache<vg::Texture2DColorAttachment>(),
            };
            m_pColorAttachmentCache = m_pMyColorAttachmentCache.get();
        }
        if (pDepthStencilAttachmentCache == nullptr) {
            m_pMyDepthStencilAttachmentCache = std::shared_ptr<TextureCache<vg::Texture2DDepthStencilAttachment>> {
                new TextureCache<vg::Texture2DDepthStencilAttachment>(),
            };
            m_pDepthStencilAttachmentCache = m_pMyDepthStencilAttachmentCache.get();
        }
    }

    void MaterialDeferred::beginBindToRender(const BindInfo info, BindResult *pResult)
    {

    }

    void MaterialDeferred::endBindToRender(const EndBindInfo info)
    {
    
    }

    vg::Pass * MaterialDeferred::getPassDeferred() const
    {
        return m_pPassDeferred.get();
    }
    
    vg::Pass * MaterialDeferred::getPassComposition() const
    {
        return m_pPassComposition.get();
    }

    void MaterialDeferred::_createAttachments(CreateInfo createInfo)
    {
        m_pDeferredAttachments.resize(createInfo.deferredAttachmentCount);
    }
        
    void MaterialDeferred::_createRenderPass(CreateInfo createInfo)
    {

    }
       
    void MaterialDeferred::_createFramebuffer(CreateInfo createInfo)
    {

    }
        
    void MaterialDeferred::_createOtherPasses(CreateInfo createInfo)
    {

    }
        
    void MaterialDeferred::_initPasses(CreateInfo createInfo)
    {

    }

} //vge