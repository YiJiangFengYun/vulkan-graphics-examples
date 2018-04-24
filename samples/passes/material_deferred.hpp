#ifndef MATERIAL_DEFERRED_HPP
#define MATERIAL_DEFERRED_HPP

#include <framework/framework.hpp>

class MaterialDeferred : public vg::Material
{
public:
    MaterialDeferred(uint32_t trunkFrameBufferWidth = 0u, uint32_t trunkFrameBufferHeight = 0u);

    virtual void beginBindToRender(const BindInfo info, BindResult *pResult) override;
    virtual void endBindToRender(const EndBindInfo info) override;

    vg::Pass * getPassDeferred() const;
    vg::Pass * getPassComposition() const;

private:
    uint32_t m_frameBufferWidth;
    uint32_t m_frameBufferHeight;
    uint32_t m_trunkFramebufferWidth;
    uint32_t m_trunkFramebufferHeight;    
    std::shared_ptr<vg::Texture2DColorAttachment> m_pAttachmentColor;
    std::shared_ptr<vg::Texture2DColorAttachment> m_pAttachmentPos;
    std::shared_ptr<vg::Texture2DColorAttachment> m_pAttachmentNormal;
    std::shared_ptr<vg::Texture2DColorAttachment> m_pAttachmentAlbedo;
    std::shared_ptr<vg::Texture2DDepthStencilAttachment> m_pAttachmentDepthStencil;
    const vg::Texture::ImageView * m_pImageViewDepth;
    std::shared_ptr<vk::RenderPass> m_pRenderPass;
    std::shared_ptr<vk::Framebuffer> m_pFrameBuffer;
    std::shared_ptr<vg::Shader> m_pShaderDeferred;
    std::shared_ptr<vg::Shader> m_pShaderComposition;
    std::shared_ptr<vg::Pass> m_pPassDeferred;
    std::shared_ptr<vg::Pass> m_pPassComposition;

    std::unordered_map<vg::InstanceID, std::shared_ptr<vg::DimSepMesh2>> m_mapRectMeshes;

    void _createAttachments();
    void _createRenderPass();
    void _createFramebuffer();
    void _createOtherPasses();
    void _initPasses();

    vg::DimSepMesh2 *_getRectMesh(vg::InstanceID id);

};

#endif //MATERIAL_DEFERRED_HPP