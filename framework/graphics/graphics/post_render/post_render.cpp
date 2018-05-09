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
        : m_pMesh()
        , m_pMaterial()
    {
        std::shared_ptr<vg::DimSepMesh2> pRectMesh = std::shared_ptr<vg::DimSepMesh2>{
            new vg::DimSepMesh2(vg::MemoryPropertyFlagBits::HOST_VISIBLE)
        };
        m_pMesh = pRectMesh;
        const uint32_t VertexCount = 0u;
        //rect pos
        std::vector<vg::Vector2> rectPoses(VertexCount);
        //rect uv
        std::vector<vg::Vector2> rectUVs(VertexCount);
        rectPoses = {
            vg::Vector2{-1.0f, -1.0f},
            vg::Vector2{1.0f, -1.0f},
            vg::Vector2{1.0f, 1.0f},
            vg::Vector2{-1.0f, 1.0f},
        };
        rectUVs = {
            vg::Vector2{0.0f, 0.0f},
            vg::Vector2{1.0f, 0.0f},
            vg::Vector2{1.0f, 1.0f},
            vg::Vector2{0.0f, 1.0f},
        };
        std::vector<uint32_t> indices = {
            0, 1, 3, 3, 1, 2
        };
        pRectMesh->setVertexCount(VertexCount);
        pRectMesh->setPositions(rectPoses);
        pRectMesh->setTextureCoordinates<vg::TextureCoordinateType::VECTOR_2, vg::TextureCoordinateIndex::TextureCoordinate_0>(
            rectUVs
        );
        pRectMesh->setIndices(indices, vg::PrimitiveTopology::TRIANGLE_LIST, 0u);
        pRectMesh->apply(VG_TRUE);
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
            vg::RenderPassInfo renderPassInfo;
            renderPassInfo.pRenderPass = nullptr;
	        renderPassInfo.pFrameBuffer = nullptr;
            renderPassInfo.framebufferWidth = info.framebufferWidth;
            renderPassInfo.framebufferHeight = info.framebufferHeight;
            renderPassInfo.projMatrix = vg::Matrix4x4(1.0f);
            renderPassInfo.viewMatrix = vg::Matrix4x4(1.0f);
            renderPassInfo.pPass = m_pMaterial->getMainPass();
            renderPassInfo.modelMatrix = vg::Matrix4x4(1.0f);
            renderPassInfo.pMesh = m_pMesh.get();
            renderPassInfo.subMeshIndex = 0u;
            renderPassInfo.viewport = fd::Viewport();
            renderPassInfo.scissor = fd::Rect2D();
    
            vg::CmdInfo cmdInfo;
            cmdInfo.pRenderPassInfo = &renderPassInfo;
            pResult->pPostRenderCmdBuffer->addCmd(cmdInfo);
        }
    }
		
    void PostRender::endBindToRender()
    {
        
    }
} //vg
