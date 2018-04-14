#ifndef MATERIAL_DEFERRED_HPP
#define MATERIAL_DEFERRED_HPP

#include <framework/framework.hpp>

class MaterialDeferred : public vg::Material
{
public:
    MaterialDeferred();
    MaterialDeferred(uint32_t trunkFrameBufferWidth, uint32_t trunkFrameBufferHeight);

    virtual void beginBindToRender(const BindInfo info, BindResult *pResult) override;
    virtual void endBindToRender(const EndBindInfo info) override;   
private:
    uint32_t m_frameBufferWidth;
    uint32_t m_frameBufferHeight;
    uint32_t m_trunkFramebufferWidth;
    uint32_t m_trunkFramebufferHeight;    
    std::shared_ptr<vg::TextureColorAttachment> m_pAttachmentColor;
    std::shared_ptr<vg::TextureColorAttachment> m_pAttachmentPos;
    std::shared_ptr<vg::TextureColorAttachment> m_pAttachmentNormal;
    std::shared_ptr<vg::TextureColorAttachment> m_pAttachmentAlbedo;
    std::shared_ptr<vg::TextureDepthStencilAttachment> m_pAttachmentDepthStencil;
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