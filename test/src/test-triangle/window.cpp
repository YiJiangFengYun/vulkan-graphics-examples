#include "test-triangle/window.hpp"

namespace testTriangle
{
	bool Window::Vertex::operator==(const Vertex& other) const {
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}

	Window::Window(uint32_t width
		, uint32_t height
		, const char* title
		, std::shared_ptr<vk::Instance> pInstance
		, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
		, std::shared_ptr<vk::Device> device
		, vk::Queue graphicsQueue
		, vk::Queue presentQueue
	)
		: gfw::Window(width
			, height
			, title
			, pInstance
			, pPhysicalDevice
			, device
			, graphicsQueue
			, presentQueue
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

	Window::Window(uint32_t width
		, uint32_t height
		, const char* title
		, RenderType renderType
		, std::shared_ptr<vk::Instance> pInstance
		, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
		, std::shared_ptr<vk::Device> pDevice
		, vk::Queue graphicsQueue
		, vk::Queue presentQueue
	)
		: gfw::Window(width
			, height
			, title
			, renderType
			, pInstance
			, pPhysicalDevice
			, pDevice
			, graphicsQueue
			, presentQueue
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
		, std::shared_ptr<vk::Instance> pInstance
		, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
		, std::shared_ptr<vk::Device> pDevice
		, vk::Queue graphicsQueue
		, vk::Queue presentQueue
	)
		: gfw::Window(pWindow
			, pSurface
			, pInstance
			, pPhysicalDevice
			, pDevice
			, graphicsQueue
			, presentQueue
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

	Window::Window(RenderType renderType
		, std::shared_ptr<GLFWwindow> pWindow
		, std::shared_ptr<vk::SurfaceKHR> pSurface
		, std::shared_ptr<vk::Instance> pInstance
		, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
		, std::shared_ptr<vk::Device> pDevice
		, vk::Queue graphicsQueue
		, vk::Queue presentQueue
	)
		: gfw::Window(renderType
			, pWindow
			, pSurface
			, pInstance
			, pPhysicalDevice
			, pDevice
			, graphicsQueue
			, presentQueue
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
		m_tempPositions = { kgs::Vector3(0.0f, -0.5f, 0.5f)
		, kgs::Vector3(-0.5f, 0.5f, 0.5f)
		, kgs::Vector3(0.5f, 0.5f, 0.5f)
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
		m_pMesh = static_cast<std::shared_ptr<kgs::Mesh3>>(new kgs::Mesh3());
		m_pMesh->setVertexCount(static_cast<uint32_t>(m_tempPositions.size()));
		m_pMesh->setPositions(m_tempPositions);
		m_pMesh->setColors(m_tempColors);
		m_pMesh->setIndices(m_tempIndices, kgs::PrimitiveTopology::TRIANGLE_LIST, 0u);
		m_pMesh->apply(KGS_TRUE);
	}

	void Window::_createMaterial()
	{
		m_pShader = std::shared_ptr<kgs::Shader>(new kgs::Shader("shaders/triangle.vert.spv", "shaders/triangle.frag.spv"));
		m_pPass = std::shared_ptr<kgs::Pass>(new kgs::Pass(m_pShader));
		m_pMaterial = std::shared_ptr<kgs::Material>(new kgs::Material());
		m_pMaterial->setPass("pass0", m_pPass);
		m_pMaterial->setRenderPriority(0u);
		m_pMaterial->setRenderQueueType(kgs::MaterialShowType::OPAQUE);
		m_pMaterial->apply();
	}

	void Window::_createModel()
	{
		m_pModel = std::shared_ptr<kgs::VisualObject3>(new kgs::VisualObject3());
		m_pModel->setMesh(m_pMesh);
		m_pModel->setMaterial(m_pMaterial);
	}

	void Window::_createCamera()
	{
		m_pCamera = std::shared_ptr<kgs::Camera3>(new kgs::Camera3());
		m_pCamera->setAspect(m_swapchainExtent.width / (float)(m_swapchainExtent.height));
		m_pCamera->setFovY(glm::radians(45.0f));
		m_pCamera->setZNear(0.1f);
		m_pCamera->setZFar(10.0f);
		m_pCamera->apply();
	}

	void Window::_createScene()
	{
		m_pScene = std::shared_ptr<kgs::Scene3>(new kgs::Scene3());
		m_pScene->setCameraWithName("MainCamera", m_pCamera);
		m_pScene->setVisualObjectWithName("Object1", m_pModel);
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
				dynamic_cast<kgs::Renderer3 *>(pRenderer.get())->reset(m_pScene, m_pCamera);
				break;
			}
			}
		}
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