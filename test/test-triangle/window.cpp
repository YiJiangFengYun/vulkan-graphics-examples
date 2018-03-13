#include "test-triangle/window.hpp"

namespace testTriangle
{
	Window::Window(uint32_t width
		, uint32_t height
		, const char* title
	)
		: testlib::Window<vg::SpaceType::SPACE_3>(width
			, height
			, title
		)
	{
		_loadModel();
		_createMesh();
		_createMaterial();
		_createModel();
	}

	Window::Window(std::shared_ptr<GLFWwindow> pWindow
		, std::shared_ptr<vk::SurfaceKHR> pSurface
	)
		: testlib::Window<vg::SpaceType::SPACE_3>(pWindow
			, pSurface
		)
	{
		_loadModel();
		_createMesh();
		_createMaterial();
		_createModel();
	}

	void Window::_loadModel()
	{
		m_tempPositions = {vg::Vector3(0.5f, 0.5f, 0.5f)
			, vg::Vector3(-0.5f, 0.5f, 0.5f)
			, vg::Vector3(0.0f, -0.5f, 0.5f)
		};

		m_tempColors = { vg::Color32(255, 0, 0, 255)
		, vg::Color32(0, 255, 0, 255)
		, vg::Color32(0, 0, 255, 255)
		};

		m_tempIndices = {
			0, 1, 2
		};
	}

	void Window::_createMesh()
	{
		m_pMesh = static_cast<std::shared_ptr<vg::DimSepMesh3>>(new vg::DimSepMesh3());
		m_pMesh->setVertexCount(static_cast<uint32_t>(m_tempPositions.size()));
		m_pMesh->setPositions(m_tempPositions);
		m_pMesh->setColors(m_tempColors);
		m_pMesh->setIndices(m_tempIndices, vg::PrimitiveTopology::TRIANGLE_LIST, 0u);
		m_pMesh->apply(VG_TRUE);
	}

	void Window::_createMaterial()
	{
		m_pShader = std::shared_ptr<vg::Shader>(new vg::Shader("shaders/triangle.vert.spv", "shaders/triangle.frag.spv"));
		m_pPass = std::shared_ptr<vg::Pass>(new vg::Pass(m_pShader.get()));
		m_pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
		m_pMaterial->addPass(m_pPass.get());
		m_pMaterial->setRenderPriority(0u);
		m_pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);
		m_pMaterial->apply();
	}

	void Window::_createModel()
	{
		m_pModel = std::shared_ptr<vg::VisualObject3>(new vg::VisualObject3());
		m_pModel->setMesh(m_pMesh.get());
		m_pModel->setMaterial(m_pMaterial.get());
		m_pScene->addVisualObject(m_pModel.get());
	}
}