#ifndef WINDOW_H
#define WINDOW_H

#include <testlib/stb_image.h>
#include <testlib/tiny_obj_loader.h>
#include <testlib/window.hpp>

namespace testTriangle
{
	class Window : public testlib::Window<vg::SpaceType::SPACE_3>
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

		std::shared_ptr<vg::VisualObject3> m_pModel;
		std::shared_ptr<vg::DimSepMesh3> m_pMesh;
		std::shared_ptr<vg::Material> m_pMaterial;
		std::shared_ptr<vg::Pass> m_pPass;
		std::shared_ptr<vg::Shader> m_pShader;


		void _loadModel();
		void _createMesh();
		void _createMaterial();
		void _createModel();
	};

}

#endif // !WINDOW_H
