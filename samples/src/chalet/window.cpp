#include "chalet/window.hpp"

#define MODEL_PATH "models/chalet.obj"
#define TEXTURE_PATH "textures/chalet.jpg"

namespace chalet
{
	SubWindow::SubWindow(uint32_t width
		, uint32_t height
		, const char* title
		, std::shared_ptr<vk::Instance> pInstance
		, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
		, std::shared_ptr<vk::Device> device, vk::Queue graphicsQueue
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
	}

	SubWindow::SubWindow(uint32_t width
		, uint32_t height
		, const char* title
		, RenderType renderType
		, std::shared_ptr<vk::Instance> pInstance
		, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
		, std::shared_ptr<vk::Device> device, vk::Queue graphicsQueue
		, vk::Queue presentQueue
	)
		: gfw::Window(width
			, height
			, title
			, renderType
			, pInstance
			, pPhysicalDevice
			, device
			, graphicsQueue
			, presentQueue
		)
	{
	}

	SubWindow::SubWindow(std::shared_ptr<GLFWwindow> pWindow
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
	}

	SubWindow::SubWindow(RenderType renderType
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

	}

	void SubWindow::_onPreUpdate()
	{

	}

	void SubWindow::_update()
	{

	}

	void SubWindow::_onPostUpdate()
	{

	}

	void SubWindow::_onPreRender()
	{

	}

	void SubWindow::_onPostRender()
	{

	}

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
		_createTexture();
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
		_createTexture();
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
		_createTexture();
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
		_createTexture();
		_createMaterial();
		_createModel();
		_createCamera();
		_createScene();
		_fillRenderer();
	}

	void Window::_loadModel()
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, MODEL_PATH))
		{
			throw std::runtime_error(err);
		}

		std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				Vertex vertex = {};

				vertex.pos = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};

				vertex.color = { 1.0f, 1.0f, 1.0f };

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(m_tempPositions.size());
					m_tempPositions.push_back(vertex.pos);
					m_tempTexCoords.push_back(vertex.texCoord);
					m_tempColors.push_back(kgs::Color(vertex.color, 1.0f));
				}

				m_tempIndices.push_back(uniqueVertices[vertex]);
			}
		}
	}

	void Window::_createMesh()
	{
		m_pMesh = static_cast<std::shared_ptr<kgs::Mesh3>>(new kgs::Mesh3());
		m_pMesh->setVertexCount(static_cast<uint32_t>(m_tempPositions.size()));
		m_pMesh->setPositions(m_tempPositions);
		m_pMesh->setUVs<kgs::UVType::VECTOR_2, kgs::UVIndex::UV_0>(m_tempTexCoords);
		m_pMesh->setIndices(m_tempIndices, kgs::PrimitiveTopology::TRIANGLE_LIST, 0u);
		m_pMesh->apply(KGS_TRUE);
	}

	void Window::_createTexture()
	{
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load(TEXTURE_PATH, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize = texWidth * texHeight * 4;

		if (!pixels) {
			throw std::runtime_error("Failed to load texture image!");
		}

		uint32_t colorCount = texWidth * texHeight;
		std::vector<kgs::Color32> colors(colorCount);
		for (uint32_t i = 0; i < colorCount; ++i)
		{
			colors[i].r = pixels[i * 4 + 0];
			colors[i].g = pixels[i * 4 + 1];
			colors[i].b = pixels[i * 4 + 2];
			colors[i].a = pixels[i * 4 + 3];
		}

		m_pTexture = std::shared_ptr<kgs::Texture2D>(new kgs::Texture2D(kgs::TextureFormat::R8G8B8A8_UNORM, KGS_FALSE, texWidth, texHeight));
		m_pTexture->setPixels32(colors);
		m_pTexture->apply(KGS_FALSE, KGS_TRUE);
	}

	void Window::_createMaterial()
	{
		m_pShader = std::shared_ptr<kgs::Shader>(new kgs::Shader("shaders/chalet.vert.spv", "shaders/chalet.frag.spv"));
		m_pPass = std::shared_ptr<kgs::Pass>(new kgs::Pass(m_pShader));
		m_pMaterial = std::shared_ptr<kgs::Material>(new kgs::Material());
		m_pMaterial->setPass("pass0", m_pPass);
		m_pMaterial->setRenderPriority(0u);
		m_pMaterial->setRenderQueueType(kgs::MaterialShowType::OPAQUE);
		m_pPass->setMainTexture(m_pTexture);

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

		auto pTransform = m_pModel->getTransform();
		//pTransform->rotateAround(kgs::Vector3(0.0f), kgs::Vector3(0.0f, 0.0f, 1.0f), glm::radians(90.0f) * time, kgs::Vector3(1.0f));
		pTransform->setLocalRotation(glm::angleAxis(glm::radians(90.0f) * time, kgs::Vector3(0.0f, 0.0f, 1.0f)));

		pTransform = m_pCamera->getTransform();
		pTransform->setLocalPosition(kgs::Vector3(2.0f, 2.0f, 2.0f));
		pTransform->lookAt(kgs::Vector3(0.0f, 0.0f, 0.0f), kgs::Vector3(0.0f, 0.0f, 1.0f));
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