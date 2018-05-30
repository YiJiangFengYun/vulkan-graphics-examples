#ifndef WINDOW_H
#define WINDOW_H

#include "sampleslib/window.hpp"
#include "sampleslib/scene_assimp.hpp"

class Window : public sampleslib::Window<vg::SpaceType::SPACE_3>
{
public:
    using ParentWindowType = sampleslib::Window<vg::SpaceType::SPACE_3>;
    
    Window(uint32_t width
        , uint32_t height
        , const char* title
    );
    Window(std::shared_ptr<GLFWwindow> pWindow
        , std::shared_ptr<vk::SurfaceKHR> pSurface
    );
private:
    bool m_wireFrame;
    sampleslib::AssimpScene m_assimpScene;
    std::shared_ptr<vg::Texture2D> m_pTexture;
    
    std::shared_ptr<vg::Material> m_pMaterialSolid;

    std::shared_ptr<vg::Material> m_pMaterialWireframe;

    struct OtherInfo 
    {
        vg::Vector4 lightPos;
    } m_otherInfo;

    virtual void _init() override;
    virtual void _initState() override;
    void _createModel();
    void _createTexture();
    void _createMaterial();
    void _initScene();

    virtual void _onUpdate() override;
};

#endif // !WINDOW_
