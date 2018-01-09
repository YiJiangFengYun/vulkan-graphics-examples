#ifndef WINDOW_H
#define WINDOW_H

#include "testlib/stb_image.h"
#include "testlib/tiny_obj_loader.h"
#include "framework/framework.hpp"

namespace testTriangle_2d
{
	class Window : public gfw::Window
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
		std::vector<kgs::Vector2> m_tempPositions;
		std::vector<kgs::Color32> m_tempColors;
		std::vector<uint32_t> m_tempIndices;


		std::shared_ptr<kgs::Scene2> m_pScene;
		std::shared_ptr<kgs::CameraOP2> m_pCamera;
		std::shared_ptr<kgs::VisualObject2> m_pModel;
		std::shared_ptr<kgs::Mesh2> m_pMesh;
		std::shared_ptr<kgs::Material> m_pMaterial;
		std::shared_ptr<kgs::Pass> m_pPass;
		std::shared_ptr<kgs::Shader> m_pShader;


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
