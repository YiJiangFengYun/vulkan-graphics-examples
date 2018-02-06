#include "chalet/window.hpp"

#define MODEL_PATH "models/chalet.obj"
#define TEXTURE_PATH "textures/chalet.jpg"

namespace chalet
{
	bool Window::Vertex::operator==(const Vertex& other) const {
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}

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
		_createTexture();
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
		_createTexture();
		_createMaterial();
		_createModel();
		_createCamera();
		_createScene();
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
					m_tempColors.push_back(vg::Color(vertex.color, 1.0f));
				}

				m_tempIndices.push_back(uniqueVertices[vertex]);
			}
		}
	}

	void Window::_createMesh()
	{
		m_pMesh = static_cast<std::shared_ptr<vg::DimSepMesh3>>(new vg::DimSepMesh3());
		m_pMesh->setVertexCount(static_cast<uint32_t>(m_tempPositions.size()));
		m_pMesh->setPositions(m_tempPositions);
		m_pMesh->setUVs<vg::UVType::VECTOR_2, vg::UVIndex::UV_0>(m_tempTexCoords);
		m_pMesh->setIndices(m_tempIndices, vg::PrimitiveTopology::TRIANGLE_LIST, 0u);
		m_pMesh->apply(VG_TRUE);
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
		std::vector<vg::Color32> colors(colorCount);
		for (uint32_t i = 0; i < colorCount; ++i)
		{
			colors[i].r = pixels[i * 4 + 0];
			colors[i].g = pixels[i * 4 + 1];
			colors[i].b = pixels[i * 4 + 2];
			colors[i].a = pixels[i * 4 + 3];
		}

		m_pTexture = std::shared_ptr<vg::Texture2D>(new vg::Texture2D(vg::TextureFormat::R8G8B8A8_UNORM, VG_FALSE, texWidth, texHeight));
		m_pTexture->setPixels32(colors);
		m_pTexture->apply(VG_FALSE, VG_TRUE);
	}

	void Window::_createMaterial()
	{
		m_pShader = std::shared_ptr<vg::Shader>(new vg::Shader("shaders/chalet.vert.spv", "shaders/chalet.frag.spv"));
		m_pPass = std::shared_ptr<vg::Pass>(new vg::Pass(m_pShader));
		m_pPass->setCullMode(vg::CullModeFlagBits::eBack);
		m_pPass->setFrontFace(vg::FrontFaceType::eClockwise);
		vg::DepthStencilInfo depthStencilStateInfo;
		depthStencilStateInfo.depthTestEnable = VG_TRUE;
		depthStencilStateInfo.depthWriteEnable = VG_TRUE;
		depthStencilStateInfo.depthCompareOp = vk::CompareOp::eLess;
		m_pPass->setDepthStencilStateInfo(depthStencilStateInfo);
		m_pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
		m_pMaterial->addPass(m_pPass);
		m_pMaterial->setRenderPriority(0u);
		m_pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);
		m_pPass->setMainTexture(m_pTexture);

	}

	void Window::_createModel()
	{
		m_pModel = std::shared_ptr<vg::VisualObject3>(new vg::VisualObject3());
		m_pModel->setMesh(m_pMesh);
		m_pModel->setMaterial(m_pMaterial);
	}

	void Window::_createCamera()
	{
		m_pCamera = std::shared_ptr<vg::Camera3>(new vg::Camera3());
		m_pCamera->setAspect(m_swapchainExtent.width / (float)(m_swapchainExtent.height));
		m_pCamera->setFovY(glm::radians(45.0f));
		m_pCamera->setZNear(0.1f);
		m_pCamera->setZFar(10.0f);
		m_pCamera->apply();
	}

	void Window::_createScene()
	{
		m_pScene = std::shared_ptr<vg::Scene3>(new vg::Scene3());
		m_pScene->addCamera(m_pCamera);
		m_pScene->addVisualObject(m_pModel);
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

	void Window::_update()
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		auto duration = currentTime - startTime;
		float time = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() / 1000.0f;

		auto pTransform = m_pModel->getTransform();
		//pTransform->rotateAround(kgs::Vector3(0.0f), kgs::Vector3(0.0f, 0.0f, 1.0f), glm::radians(90.0f) * time, kgs::Vector3(1.0f));
		pTransform->setLocalRotation(glm::angleAxis(glm::radians(90.0f) * time, vg::Vector3(0.0f, 0.0f, 1.0f)));

		pTransform = m_pCamera->getTransform();
		//pTransform->setLocalPosition(kgs::Vector3(2.0f, 2.0f, 2.0f));
		pTransform->lookAt2(vg::Vector3(2.0f, 2.0f, 2.0f), vg::Vector3(0.0f, 0.0f, 0.0f), vg::Vector3(0.0f, 0.0f, 1.0f));
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
		myInfo.countSceneAndCamera = 1u;
		myInfo.pSceneAndCamera = &sceneAndCamera;
		pRenderer->render(myInfo, resultInfo);
	}
}