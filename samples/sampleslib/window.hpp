#ifndef SAMPLES_LIB_WINDOW_H
#define SAMPLES_LIB_WINDOW_H

#include <framework/framework.hpp>

namespace sampleslib
{
    template<vg::SpaceType type>
    struct SpaceObjectInfo
    {
        using SceneType = void;
        using CameraType = void;
    };

    template<>
    struct SpaceObjectInfo<vg::SpaceType::SPACE_2>
    {
        using SceneType = vg::Scene2;
        using CameraType = vg::CameraOP2;
    };

    template<>
    struct SpaceObjectInfo<vg::SpaceType::SPACE_3>
    {
        using SceneType = vg::Scene3;
        using CameraType = vg::Camera3;
    };

    template <vg::SpaceType SPACE_TYPE>
    class Window : public vgf::Window
    {
    public:
        using PointType = typename vg::SpaceTypeInfo<SPACE_TYPE>::PointType;
        using RotationType = typename vg::SpaceTypeInfo<SPACE_TYPE>::RotationType;
        using RotationDimType = typename vg::SpaceTypeInfo<SPACE_TYPE>::RotationDimType;
        using SceneType = typename SpaceObjectInfo<SPACE_TYPE>::SceneType;
        using CameraType = typename SpaceObjectInfo<SPACE_TYPE>::CameraType;
        using TimePointType = typename std::chrono::time_point<std::chrono::steady_clock>;

        Window(uint32_t width
            , uint32_t height
            , const char* title
        );

        Window(std::shared_ptr<GLFWwindow> pWindow
            , std::shared_ptr<vk::SurfaceKHR> pSurface
        );
    protected:
        
        float m_rotationSpeed;

        TimePointType m_startTimeFrame;
        TimePointType m_endTimeFrame;
        bool m_isPause;
        /**
         * Costing time of per frame (s)
         */
        float m_frameTimer;
        /**
         * Passed time since starting application (s)
         **/
        float m_passedTime;
        /**
         * The speed of time advaced [0, 1]
         **/
        float m_timerSpeedFactor;

        uint32_t m_fpsTimer;
        uint32_t m_frameCounter;
        uint32_t m_lastFPS;
        uint32_t m_lastDrawCount;

        vg::Vector2 m_lastWinPos;
        vg::Vector2 m_lastWinSize;

        float m_cameraZoom;
        float m_cameraZoomSpeed;
        float m_cameraAspect;
        PointType m_cameraPosition;
        RotationDimType m_cameraRotation;
        RotationDimType m_worldRotation;
        
        struct  {
            vgf::Bool32 left = VGF_FALSE;
            vgf::Bool32 right = VGF_FALSE;
            vgf::Bool32 middle = VGF_FALSE;
        } m_mouseButtons;
        double m_mousePos[2];

        uint32_t m_sceneCount;
        std::vector<std::shared_ptr<SceneType>> m_pScenes;
        std::shared_ptr<SceneType> m_pScene;
        std::shared_ptr<CameraType> m_pCamera;
        vg::Bool32 m_preDepthScene;

        virtual void _onResize() override;
        virtual void _onPreReCreateSwapchain() override;
        virtual void _onPostReCreateSwapchain() override;

        virtual void _onPreUpdate() override;
        virtual void _onUpdate() override;
        virtual void _onPostUpdate() override;

        virtual void _onPreDraw() override;
        virtual void _onDraw() override;
        virtual void _onPostDraw() override;

        static std::vector<vg::Renderer::SceneInfo> mySceneInfos;
        
        virtual void _onPreRender(vg::Renderer::RenderInfo &info
            , vg::Renderer::RenderResultInfo &resultInfo) override;
        virtual void _onRender(vg::Renderer::RenderInfo &info
            , vg::Renderer::RenderResultInfo &resultInfo) override;
        virtual void _onPostRender(vg::Renderer::RenderInfo &info
            , vg::Renderer::RenderResultInfo &resultInfo) override;

        virtual void _init();
        virtual void _initState();
        
    private:
        void _createCamera();
        void _createScene();
        void _initUI();
        void _initInputHanders();

        void _updateCamera();
    };

}

#include "sampleslib/window.inl"

#endif // !SAMPLES_LIB_WINDOW_H
