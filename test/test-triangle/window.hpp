#ifndef WINDOW_H
#define WINDOW_H

#include "testlib/stb_image.h"
#include "testlib/tiny_obj_loader.h"
#include "framework/framework.hpp"

namespace testTriangle
{
	class Window : public vgf::Window
	{
	public:

		Window(uint32_t width
			, uint32_t height
			, const char* title
		);

		Window(std::shared_ptr<GLFWwindow> pWindow
			, std::shared_ptr<vk::SurfaceKHR> pSurface
		);

	private:
		std::vector<vg::Vector3> m_tempPositions;
		std::vector<vg::Color32> m_tempColors;
		std::vector<uint32_t> m_tempIndices;


		std::shared_ptr<vg::Scene3> m_pScene;
		std::shared_ptr<vg::CameraOP3> m_pCamera;
		std::shared_ptr<vg::VisualObject3> m_pModel;
		std::shared_ptr<vg::Mesh3> m_pMesh;
		std::shared_ptr<vg::Material> m_pMaterial;
		std::shared_ptr<vg::Pass> m_pPass;
		std::shared_ptr<vg::Shader> m_pShader;


		void _loadModel();
		void _createMesh();
		void _createMaterial();
		void _createModel();
		void _createCamera();
		void _createScene();
		void _fillRenderer();

		void _onPreReCreateSwapchain() override;
		void _onPostReCreateSwapchain() override;

		void _onPreUpdate() override;
		void _update() override;
		void _onPostUpdate() override;

		void _onPreRender() override;
	    void _onPostRender() override;
	};

}

#endif // !WINDOW_H
