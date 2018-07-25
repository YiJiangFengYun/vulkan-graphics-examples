#include "graphics/material/material_default.hpp"

namespace vg
{
    std::shared_ptr<Material> pDefaultPreDepthMaterial = nullptr;
    std::shared_ptr<Material> pDefaultLightDepthMaterial = nullptr;
    std::shared_ptr<Material> pDefaultLightPointDistMaterial = nullptr;

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
            dynamic_cast<MaterialForDefault *>(pDefaultPreDepthMaterial.get())->setMainPass(pDefaultLightDepthPass.get());
        }

        //Light depth material.
        {
            pDefaultLightDepthMaterial = std::shared_ptr<Material>{
                new MaterialForDefault()
            };
            dynamic_cast<MaterialForDefault *>(pDefaultLightDepthMaterial.get())->setMainPass(pDefaultLightDepthPass.get());
        }

        //Light point distance material
        {
            pDefaultLightPointDistMaterial = std::shared_ptr<Material>{
                new MaterialForDefault()
            };
            dynamic_cast<MaterialForDefault *>(pDefaultLightPointDistMaterial.get())->setMainPass(pDefaultLightPointDistPass.get());
        }
        
    }

    void destroyDefaultMaterials()
    {
        pDefaultPreDepthMaterial = nullptr;
        pDefaultLightDepthMaterial = nullptr;
        pDefaultLightPointDistMaterial = nullptr;
    }
} //vg