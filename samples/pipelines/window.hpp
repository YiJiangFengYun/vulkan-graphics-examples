#ifndef WINDOW_H
#define WINDOW_H

#include "sampleslib/window.hpp"
#include "sampleslib/scene_assimp.hpp"

#define SCENE_COUNT 3u
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
    std::array<std::shared_ptr<vg::Material>, SCENE_COUNT> m_pMaterials;
    std::array<std::shared_ptr<vg::VisualObject3>, SCENE_COUNT> m_pVisualObjects;

    struct LightInfo 
    {
        vg::Vector4 lightPos;
    } m_lightInfo;

    virtual void _init() override;
    virtual void _initState() override;
    void _loadAssimpScene();
    void _createMaterial();
    void _createVisualObjects();
    void _fillScene();

    virtual void _onUpdate() override;
};

#endif // !WINDOW_
