#include "graphics/post_render/post_render.hpp"

namespace vg
{
    PostRender::BindInfo::BindInfo( uint32_t framebufferWidth
        , uint32_t framebufferHeight
        )
        : framebufferWidth(framebufferWidth)
        , framebufferHeight(framebufferHeight)
    {

    }

    PostRender::BindResult::BindResult(CmdBuffer *pPostRenderCmdBuffer
        )
        : pPostRenderCmdBuffer(pPostRenderCmdBuffer)
    {

    }

    PostRender::PostRender()
        : m_pMaterial()
    {
    }

    const Material * PostRender::getMaterial() const
    {
        return m_pMaterial;
    }

    Material * PostRender::getMaterial()
    {
        return m_pMaterial;
    }
        
    void PostRender::setMaterial(Material * pMaterial)
    {
        m_pMaterial = pMaterial;
    }

    Bool32 PostRender::isValidBindToRender() const
    {
        if (m_pMaterial != nullptr) 
        {
            return VG_TRUE;
        }
        else
        {
            return VG_FALSE;
        }
    }

    void PostRender::beginBind(const BindInfo info, BindResult *pResult) const
    {
        if (pResult->pPostRenderCmdBuffer != nullptr && 
            m_pMaterial != nullptr
            ) 
        {
            auto pPass = m_pMaterial->getMainPass();
            vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState = {
                vk::PipelineInputAssemblyStateCreateFlags(),
                vk::PrimitiveTopology::eTriangleList
            };
    
            pPass->setDefaultInputAssemblyState(inputAssemblyState);
            CmdDraw cmdDraw = {
                3,
                1,
                0,
                0,
            };
            vg::RenderPassInfo renderPassInfo;
            renderPassInfo.pRenderPass = nullptr;
            renderPassInfo.pFramebuffer = nullptr;
            renderPassInfo.framebufferWidth = info.framebufferWidth;
            renderPassInfo.framebufferHeight = info.framebufferHeight;
            renderPassInfo.projMatrix = vg::Matrix4x4(1.0f);
            renderPassInfo.viewMatrix = vg::Matrix4x4(1.0f);
            renderPassInfo.pPass = pPass;
            renderPassInfo.modelMatrix = vg::Matrix4x4(1.0f);
            renderPassInfo.pMesh = nullptr;
            renderPassInfo.subMeshIndex = 0u;
            renderPassInfo.viewport = fd::Viewport();
            renderPassInfo.scissor = fd::Rect2D();
            renderPassInfo.pCmdDraw = &cmdDraw;
    
            vg::CmdInfo cmdInfo;
            cmdInfo.pRenderPassInfo = &renderPassInfo;
            pResult->pPostRenderCmdBuffer->addCmd(cmdInfo);
        }
    }
        
    void PostRender::endBind() const
    {
        
    }
} //vg
