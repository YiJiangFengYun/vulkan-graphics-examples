#include "graphics/material/material_default.hpp"

namespace vg
{
    std::shared_ptr<MaterialPreDepthDefault> pDefaultPreDepthMaterial = nullptr;
    std::shared_ptr<MaterialLightingDepthDefault> pDefaultLightingDepthMaterial = nullptr;
    std::shared_ptr<MaterialLightingPointDistDefault> pDefaultLightingPointDistMaterial = nullptr;

    MaterialPreDepthDefault::MaterialPreDepthDefault()
        : Material(VG_FALSE)
    {
        _removePass(m_pMainPass);
        m_pMyMainShader = pDefaultPreDepthShader;
        m_pMainShader = m_pMyMainShader.get();
        m_pMyMainPass = pDefaultPreDepthPass;
        m_pMainPass = m_pMyMainPass.get();
        _addPass(m_pMainPass);
    }

    MaterialLightingDepthDefault::MaterialLightingDepthDefault()
        : Material(VG_FALSE)
    {
        _removePass(m_pMainPass);
        m_pMyMainShader = pDefaultLightingDepthShader;
        m_pMainShader = m_pMyMainShader.get();
        m_pMyMainPass = pDefaultLightingDepthPass;
        m_pMainPass = m_pMyMainPass.get();
        _addPass(m_pMainPass);
    }

    MaterialLightingPointDistDefault::MaterialLightingPointDistDefault()
        : Material(VG_FALSE)
    {
        _removePass(m_pMainPass);
        m_pMyMainShader = pDefaultLightingPointDistShader;
        m_pMainShader = m_pMyMainShader.get();
        m_pMyMainPass = pDefaultLightingPointDistPass;
        m_pMainPass = m_pMyMainPass.get();
        _addPass(m_pMainPass);
    }

    void MaterialLightingPointDistDefault::_beginBind(const BindInfo info, BindResult *pResult) const
    {
        for (uint32_t i = 0; i < 6u; ++i)
        {
            vg::RenderPassInfo trunkRenderPassInfo;
            trunkRenderPassInfo.pRenderPass = nullptr;
            trunkRenderPassInfo.pFramebuffer = nullptr;
            trunkRenderPassInfo.subPassIndex = i;
            trunkRenderPassInfo.framebufferWidth = info.framebufferWidth;
            trunkRenderPassInfo.framebufferHeight = info.framebufferHeight;
            trunkRenderPassInfo.projMatrix = *(info.pProjMatrix);
            trunkRenderPassInfo.viewMatrix = *(info.pViewMatrix);
            trunkRenderPassInfo.pPass = m_pMainPass;
            trunkRenderPassInfo.modelMatrix = *(info.pModelMatrix);
            trunkRenderPassInfo.pMesh = info.pMesh;
            trunkRenderPassInfo.subMeshIndex = info.subMeshIndex;
            trunkRenderPassInfo.viewport = fd::Viewport();
            trunkRenderPassInfo.scissor = info.hasClipRect ? info.clipRect : fd::Rect2D();

            vg::CmdInfo cmdInfo;
            cmdInfo.pRenderPassInfo = &trunkRenderPassInfo;

            pResult->pTrunkRenderPassCmdBuffer->addCmd(cmdInfo);
        }
    }

    void createDefaultMaterials()
    {
        //Pre depth material.
        {
            pDefaultPreDepthMaterial = std::shared_ptr<MaterialPreDepthDefault>{
                new MaterialPreDepthDefault()
            };
        }

        //Light depth material.
        {
            pDefaultLightingDepthMaterial = std::shared_ptr<MaterialLightingDepthDefault>{
                new MaterialLightingDepthDefault()
            };
        }

        //Light point distance material
        {
            pDefaultLightingPointDistMaterial = std::shared_ptr<MaterialLightingPointDistDefault>{
                new MaterialLightingPointDistDefault()
            };
        }
        
    }

    void destroyDefaultMaterials()
    {
        pDefaultPreDepthMaterial = nullptr;
        pDefaultLightingDepthMaterial = nullptr;
        pDefaultLightingPointDistMaterial = nullptr;
    }
} //vg