#include "graphics/material/material_default.hpp"

namespace vg
{
    std::shared_ptr<Material> pDefaultPreDepthMaterial = nullptr;
    std::shared_ptr<Material> pDefaultLightingDepthMaterial = nullptr;
    std::shared_ptr<Material> pDefaultLightingPointDistMaterial = nullptr;

    class MaterialForDefault : public vg::Material
    {
    public:
        MaterialForDefault()
            : vg::Material()
        {

        }

        void setMainPass(Pass *pPass)
        {
            //Remove default main pass created from class Material.
            _removePass(m_pMainPass);
            m_pMainShader = pPass->getShader();
            m_pMainPass = pPass;
            _addPass(m_pMainPass);
        }
    private:
    };

    void createDefaultMaterials()
    {
        //Pre depth material.
        {
            pDefaultPreDepthMaterial = std::shared_ptr<Material>{
                new MaterialForDefault()
            };
            dynamic_cast<MaterialForDefault *>(pDefaultPreDepthMaterial.get())->setMainPass(pDefaultLightingDepthPass.get());
        }

        //Light depth material.
        {
            pDefaultLightingDepthMaterial = std::shared_ptr<Material>{
                new MaterialForDefault()
            };
            dynamic_cast<MaterialForDefault *>(pDefaultLightingDepthMaterial.get())->setMainPass(pDefaultLightingDepthPass.get());
        }

        //Light point distance material
        {
            pDefaultLightingPointDistMaterial = std::shared_ptr<Material>{
                new MaterialForDefault()
            };
            dynamic_cast<MaterialForDefault *>(pDefaultLightingPointDistMaterial.get())->setMainPass(pDefaultLightingPointDistPass.get());
        }
        
    }

    void destroyDefaultMaterials()
    {
        pDefaultPreDepthMaterial = nullptr;
        pDefaultLightingDepthMaterial = nullptr;
        pDefaultLightingPointDistMaterial = nullptr;
    }
} //vg