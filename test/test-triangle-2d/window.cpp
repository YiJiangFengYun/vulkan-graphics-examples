#include "test-triangle-2d/window.hpp"

namespace testTriangle2D
{
	Window::Window(uint32_t width
		, uint32_t height
		, const char* title
	)
		: testlib::Window<vg::SpaceType::SPACE_2>(width
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
		: testlib::Window<vg::SpaceType::SPACE_2>(pWindow
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
		m_tempPositions = {vg::Vector2(0.5f, 0.5f)
		    , vg::Vector2(-0.5f, 0.5f)
		    , vg::Vector2(0.0f, -0.5f)
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
		m_pMesh = static_cast<std::shared_ptr<vg::DimSepMesh2>>(new vg::DimSepMesh2());
		m_pMesh->setVertexCount(static_cast<uint32_t>(m_tempPositions.size()));
		m_pMesh->setPositions(m_tempPositions);
		m_pMesh->setColors(m_tempColors);
		m_pMesh->setIndices(m_tempIndices, vg::PrimitiveTopology::TRIANGLE_LIST, 0u);
		m_pMesh->apply(VG_TRUE);
	}

	void Window::_createMaterial()
	{
		m_pShader = std::shared_ptr<vg::Shader>(new vg::Shader("shaders/triangle-2d.vert.spv", "shaders/triangle-2d.frag.spv"));
		m_pPass = std::shared_ptr<vg::Pass>(new vg::Pass(m_pShader.get()));
		m_pPass->setCullMode(vg::CullModeFlagBits::NONE);
		m_pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
		m_pMaterial->addPass(m_pPass.get());
		m_pMaterial->setRenderPriority(0u);
		m_pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);
		m_pMaterial->apply();
	}

	void Window::_createModel()
	{
		m_pModel = std::shared_ptr<vg::VisualObject2>(new vg::VisualObject2());
		m_pModel->setMesh(m_pMesh.get());
		m_pModel->setMaterial(m_pMaterial.get());
		m_pScene->addVisualObject(m_pModel.get());
	}
}