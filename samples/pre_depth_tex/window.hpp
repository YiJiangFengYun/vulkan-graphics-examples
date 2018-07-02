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
    sampleslib::AssimpScene m_assimpScene;
    std::shared_ptr<vg::Material> m_pMaterial;

    struct OtherInfo 
    {
        float zFar;
        float zNear;
    } m_otherInfo;

    virtual void _init() override;
    virtual void _initState() override;
    void _createModel();
    void _createMaterial();
    void _initScene();
    void _enablePreDepth();

    virtual void _onUpdate() override;
};

#endif // !WINDOW_
