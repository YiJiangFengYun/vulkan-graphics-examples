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
        
    void PostRender::setMaterial(const Material * pMaterial)
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

    void PostRender::beginBindToRender(const BindInfo info, BindResult *pResult)
    {
        if (pResult->pPostRenderCmdBuffer != nullptr && 
            m_pMaterial != nullptr
            ) 
        {
            CmdDraw cmdDraw = {
                3,
                1,
                0,
                0,
            };
            vg::RenderPassInfo renderPassInfo;
            renderPassInfo.pRenderPass = nullptr;
	        renderPassInfo.pFrameBuffer = nullptr;
            renderPassInfo.framebufferWidth = info.framebufferWidth;
            renderPassInfo.framebufferHeight = info.framebufferHeight;
            renderPassInfo.projMatrix = vg::Matrix4x4(1.0f);
            renderPassInfo.viewMatrix = vg::Matrix4x4(1.0f);
            renderPassInfo.pPass = m_pMaterial->getMainPass();
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
		
    void PostRender::endBindToRender()
    {
        
    }
} //vg
