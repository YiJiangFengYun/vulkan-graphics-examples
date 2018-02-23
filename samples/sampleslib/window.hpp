#ifndef SAMPLES_LIB_WINDOW_H
#define SAMPLES_LIB_WINDOW_H

#include <framework/framework.hpp>

namespace sampleslib
{
	template<vg::SpaceType type>
	struct SpaceObjectInfo
	{
		typedef void SceneType;
		typedef void CameraType;
	};

	template<>
	struct SpaceObjectInfo<vg::SpaceType::SPACE_2>
	{
		typedef vg::Scene2 SceneType;
		typedef vg::CameraOP2 CameraType;
	};

	template<>
	struct SpaceObjectInfo<vg::SpaceType::SPACE_3>
	{
		typedef vg::Scene3 SceneType;
		typedef vg::Camera3 CameraType;
	};

	template <vg::SpaceType SPACE_TYPE>
	class Window : public vgf::Window
	{
	public:
	    typedef typename vg::SpaceTypeInfo<SPACE_TYPE>::RotationType RotationType;
	    typedef typename SpaceObjectInfo<SPACE_TYPE>::SceneType SceneType;
	    typedef typename SpaceObjectInfo<SPACE_TYPE>::CameraType CameraType;

		Window(uint32_t width
			, uint32_t height
			, const char* title
		);

		Window(std::shared_ptr<GLFWwindow> pWindow
			, std::shared_ptr<vk::SurfaceKHR> pSurface
		);
	protected:
		float m_zoom;
		float m_zoomSpeed;
		RotationType m_rotation;

		std::shared_ptr<SceneType> m_pScene;
		std::shared_ptr<CameraType> m_pCamera;

		virtual void _onResize() override;
		virtual void _onPreReCreateSwapchain() override;
		virtual void _onPostReCreateSwapchain() override;

		virtual void _onPreUpdate() override;
		virtual void _onUpdate() override;
		virtual void _onPostUpdate() override;

		virtual void _onPreRender() override;
		virtual void _onRender() override;
		virtual void _onPostRender() override;

		virtual void _renderWithRenderer(const std::shared_ptr<vg::Renderer> &pRenderer
			, const vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo) override;
	private:
		void _initZoom();
		void _createCamera();
		void _createScene();
		void _initUI();
		void _initInputHanders();

		void _updateCamera();
	};

}

#include "sampleslib/window.inl"

#endif // !SAMPLES_LIB_WINDOW_H
