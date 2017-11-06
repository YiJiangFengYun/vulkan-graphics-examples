#include "test-triangle-2d/window.hpp"

namespace testTriangle_2d
{
	Window::Window(uint32_t width
		, uint32_t height
		, const char* title
	)
		: gfw::Window(width
			, height
			, title
			, RenderType::RENDERER_2
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
		: gfw::Window(RenderType::RENDERER_2
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
		m_tempPositions = { kgs::Vector2(0.0f, -0.5f)
		, kgs::Vector2(-0.5f, 0.5f)
		, kgs::Vector2(0.5f, 0.5f)
		};

		m_tempColors = { kgs::Color32(255, 0, 0, 255)
		, kgs::Color32(0, 255, 0, 255)
		, kgs::Color32(0, 0, 255, 255)
		};

		m_tempIndices = {
			0, 1, 2
		};
	}

	void Window::_createMesh()
	{
		m_pMesh = static_cast<std::shared_ptr<kgs::Mesh2>>(new kgs::Mesh2());
		m_pMesh->setVertexCount(static_cast<uint32_t>(m_tempPositions.size()));
		m_pMesh->setPositions(m_tempPositions);
		m_pMesh->setColors(m_tempColors);
		m_pMesh->setIndices(m_tempIndices, kgs::PrimitiveTopology::TRIANGLE_LIST, 0u);
		m_pMesh->apply(KGS_TRUE);
	}

	void Window::_createMaterial()
	{
		m_pShader = std::shared_ptr<kgs::Shader>(new kgs::Shader("shaders/triangle-2d.vert.spv", "shaders/triangle-2d.frag.spv"));
		m_pPass = std::shared_ptr<kgs::Pass>(new kgs::Pass(m_pShader));
		m_pMaterial = std::shared_ptr<kgs::Material>(new kgs::Material());
		m_pMaterial->addPass(m_pPass);
		m_pMaterial->setRenderPriority(0u);
		m_pMaterial->setRenderQueueType(kgs::MaterialShowType::OPAQUE);
		m_pMaterial->apply();
	}

	void Window::_createModel()
	{
		m_pModel = std::shared_ptr<kgs::VisualObject2>(new kgs::VisualObject2());
		m_pModel->setMesh(m_pMesh);
		m_pModel->setMaterial(m_pMaterial);
	}

	void Window::_createCamera()
	{
		m_pCamera = std::shared_ptr<kgs::CameraOP2>(new kgs::CameraOP2());
		m_pCamera->updateProj({ {-1.0f, -1.0f}, {1.0f, 1.0f} });
		m_pCamera->apply();
	}

	void Window::_createScene()
	{
		m_pScene = std::shared_ptr<kgs::Scene2>(new kgs::Scene2());
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
				dynamic_cast<kgs::Renderer2 *>(pRenderer.get())->reset(m_pScene, m_pCamera);
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
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		auto duration = currentTime - startTime;
		float time = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() / 1000.0f;
		float strength = remainder(time, 1.0f) / 1.0f;
		m_pPass->setMainColor(kgs::Color(strength, strength, strength, 1.0f));

		for (const auto& pRenderer : m_pRenderers)
		{
			pRenderer->setClearValueColor(kgs::Color(strength, 0.0f, 0.0f, 0.0f));
		}
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