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