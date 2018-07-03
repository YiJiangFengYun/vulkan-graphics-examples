#ifndef WINDOW_H
#define WINDOW_H

#include "sampleslib/window.hpp"
#include "sampleslib/scene_assimp.hpp"

#define MAX_LIGHT_COUNT 10

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
    std::shared_ptr<vg::LightPoint3> m_pPointLight;

    virtual void _init() override;
    virtual void _initState() override;
    void _createModel();
    void _createLights();
    void _createMaterial();
    void _initScene();
    virtual void _onUpdate() override;
};

#endif // !WINDOW_
