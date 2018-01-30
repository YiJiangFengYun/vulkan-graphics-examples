#include "test-triangle/window.hpp"

namespace testTriangle
{
	Window::Window(uint32_t width
		, uint32_t height
		, const char* title
	)
		: vgf::Window(width
			, height
			, title
			, RenderType::RENDERER_3
		)
	{
		_loadModel();
		_createMesh();
		_createMaterial();
		_createModel();
		_createCamera();
		_createScene();
		_fillRenderer();
	}

	Window::Window(std::shared_ptr<GLFWwindow> pWindow
		, std::shared_ptr<vk::SurfaceKHR> pSurface
	)
		: vgf::Window(RenderType::RENDERER_3
			, pWindow
			, pSurface
		)
	{
		_loadModel();
		_createMesh();
		_createMaterial();
		_createModel();
		_createCamera();
		_createScene();
		_fillRenderer();
	}

	void Window::_loadModel()
	{
		m_tempPositions = { vg::Vector3(0.0f, -0.5f, 0.5f)
		, vg::Vector3(-0.5f, 0.5f, 0.5f)
		, vg::Vector3(0.5f, 0.5f, 0.5f)
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
		m_pMesh = static_cast<std::shared_ptr<vg::Mesh3>>(new vg::Mesh3());
		m_pMesh->setVertexCount(static_cast<uint32_t>(m_tempPositions.size()));
		m_pMesh->setPositions(m_tempPositions);
		m_pMesh->setColors(m_tempColors);
		m_pMesh->setIndices(m_tempIndices, vg::PrimitiveTopology::TRIANGLE_LIST, 0u);
		m_pMesh->apply(VG_TRUE);
	}

	void Window::_createMaterial()
	{
		m_pShader = std::shared_ptr<vg::Shader>(new vg::Shader("shaders/triangle.vert.spv", "shaders/triangle.frag.spv"));
		m_pPass = std::shared_ptr<vg::Pass>(new vg::Pass(m_pShader));
		m_pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
		m_pMaterial->addPass(m_pPass);
		m_pMaterial->setRenderPriority(0u);
		m_pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);
		m_pMaterial->apply();
	}

	void Window::_createModel()
	{
		m_pModel = std::shared_ptr<vg::VisualObject3>(new vg::VisualObject3());
		m_pModel->setMesh(m_pMesh);
		m_pModel->setMaterial(m_pMaterial);
	}

	void Window::_createCamera()
	{
		m_pCamera = std::shared_ptr<vg::CameraOP3>(new vg::CameraOP3());
		m_pCamera->updateProj({{ -1.0f, -1.0f, 0.1f }, { 1.0f, 1.0f, 10.0f }});
		m_pCamera->apply();
	}

	void Window::_createScene()
	{
		m_pScene = std::shared_ptr<vg::Scene3>(new vg::Scene3());
		m_pScene->addCamera(m_pCamera);
		m_pScene->addVisualObject(m_pModel);
	}

	void Window::_fillRenderer()
	{
		for (auto& pRenderer : m_pRenderers)
		{
			switch (m_renderType)
			{
			case RenderType::RENDERER_2:
			{
				//dynamic_cast<kgs::Renderer2 *>(pRenderer.get())->reset(m_pScene, m_pCamera);
				break;
			}
			case RenderType::RENDERER_3:
			{
				dynamic_cast<vg::Renderer3 *>(pRenderer.get())->reset(m_pScene, m_pCamera);
				break;
			}
			}
		}
	}

	void Window::_onPreReCreateSwapchain()
	{

	}

	void Window::_onPostReCreateSwapchain()
	{
		_fillRenderer();
	}

	void Window::_onPreUpdate()
	{

	}

	void Window::_update()
	{
		/*static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		auto duration = currentTime - startTime;
		float time = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() / 2000.0f;

		auto pTransform = m_pModel->getTransform();
		pTransform->setLocalRotation(glm::angleAxis(glm::radians(10.0f) * time, vg::Vector3(0.0f, 0.0f, 1.0f)));*/
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
}