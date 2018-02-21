#include "test-triangle-2d/window.hpp"

namespace testTriangle2D
{
	Window::Window(uint32_t width
		, uint32_t height
		, const char* title
	)
		: vgf::Window(width
			, height
			, title
		)
	{
		_loadModel();
		_createMesh();
		_createMaterial();
		_createModel();
		_createCamera();
		_createScene();
	}

	Window::Window(std::shared_ptr<GLFWwindow> pWindow
		, std::shared_ptr<vk::SurfaceKHR> pSurface
	)
		: vgf::Window(pWindow
			, pSurface
		)
	{
		_loadModel();
		_createMesh();
		_createMaterial();
		_createModel();
		_createCamera();
		_createScene();
	}

	void Window::_loadModel()
	{
		m_tempPositions = { vg::Vector2(0.0f, -0.5f)
		, vg::Vector2(-0.5f, 0.5f)
		, vg::Vector2(0.5f, 0.5f)
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
		m_pPass = std::shared_ptr<vg::Pass>(new vg::Pass(m_pShader));
		m_pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
		m_pMaterial->addPass(m_pPass);
		m_pMaterial->setRenderPriority(0u);
		m_pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);
		m_pMaterial->apply();
	}

	void Window::_createModel()
	{
		m_pModel = std::shared_ptr<vg::VisualObject2>(new vg::VisualObject2());
		m_pModel->setMesh(m_pMesh);
		m_pModel->setMaterial(m_pMaterial);
	}

	void Window::_createCamera()
	{
		m_pCamera = std::shared_ptr<vg::CameraOP2>(new vg::CameraOP2());
		m_pCamera->updateProj({ {-1.0f, -1.0f}, {1.0f, 1.0f} });
		m_pCamera->apply();
	}

	void Window::_createScene()
	{
		m_pScene = std::shared_ptr<vg::Scene2>(new vg::Scene2());
		m_pScene->addCamera(m_pCamera);
		m_pScene->addVisualObject(m_pModel);
	}

	void Window::_onResize()
	{

	}

	void Window::_onPreReCreateSwapchain()
	{

	}

	void Window::_onPostReCreateSwapchain()
	{
	}

	void Window::_onPreUpdate()
	{

	}

	void Window::_onUpdate()
	{
	}

	void Window::_onPostUpdate()
	{

	}

	void Window::_onPreRender()
	{

	}

	void Window::_onPostRender()
	{

	}

	void Window::_renderWithRenderer(const std::shared_ptr<vg::Renderer> &pRenderer
		    , const vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo)
	{
		vg::Renderer::SceneAndCamera sceneAndCamera;
		sceneAndCamera.pScene = m_pScene.get();
		sceneAndCamera.pCamera = m_pCamera.get();
		auto myInfo = info;
		myInfo.sceneAndCameraCount = 1u;
		myInfo.pSceneAndCamera = &sceneAndCamera;
		pRenderer->render(myInfo, resultInfo);
	}
}