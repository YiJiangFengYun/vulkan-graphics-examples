#ifndef WINDOW_H
#define WINDOW_H

#include "sampleslib/window.hpp"
#include "sampleslib/scene_assimp.hpp"

#define FB_DIM 512
#define FB_COLOR_FORMAT vk::Format::eR8G8B8A8Unorm

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
    sampleslib::AssimpScene m_assimpSceneModel;
    sampleslib::AssimpScene m_assimpScenePlane;

    std::vector<std::shared_ptr<vg::VisualObject3>> m_pVisualObjects;
    std::vector<std::shared_ptr<vg::VisualObject3>> m_pVisualObjectOffscreens;
    std::shared_ptr<vg::Scene3> m_pSceneOffScreen;
    std::shared_ptr<vg::Camera3> m_pCameraOffScreen;
    std::shared_ptr<vg::Texture2DColorAttachment> m_pOffScreenTex;
    std::shared_ptr<vg::ColorTexRendererTarget> m_pOffScreenRendererTarget;
    std::shared_ptr<vg::Renderer> m_pOffScreenRenderer;
    
    std::shared_ptr<vg::Material> m_pMaterialModelOffscreen;

    std::shared_ptr<vg::Material> m_pMaterialModel;

    std::shared_ptr<vg::Texture2D> m_pTexturePlane;    
    std::shared_ptr<vg::Material> m_pMaterialPlane;

    struct OtherInfo
    {
        vg::Vector4 lightPos;
    } m_otherInfo;

    struct OtherInfoOffScreen
    {
        vg::Matrix4x4 matrixInverse;
        vg::Vector4 lightPos;
    } m_otherInfoOffScreen;

    virtual void _init() override;
    virtual void _initState() override;
    void _createModel();
    void _createSceneOffScreen();
    void _createOffscreenTex();
    void _createOffscreenRenderer();
    void _createTexture();
    void _createMaterial();
    void _createVisualObjects();
    void _initScene();

    virtual void _onUpdate() override;

    virtual void _doRender(vg::Renderer::RenderInfo &info
            , vg::Renderer::RenderResultInfo &resultInfo) override;
};

#endif // !WINDOW_
