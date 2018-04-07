#ifndef TEST_LIB_WINDOW_H
#define TEST_LIB_WINDOW_H

#include <framework/framework.hpp>

namespace testlib
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
	    using RotationType = typename vg::SpaceTypeInfo<SPACE_TYPE>::RotationType;
	    using SceneType = typename SpaceObjectInfo<SPACE_TYPE>::SceneType;
	    using CameraType = typename SpaceObjectInfo<SPACE_TYPE>::CameraType;

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

		virtual void _render(const vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo) override;
	private:
		void _initZoom();
		void _createCamera();
		void _createScene();
		void _initInputHanders();

		void _updateCamera();
	};

}

#include "testlib/window.inl"

#endif // !TEST_LIB_WINDOW_H
