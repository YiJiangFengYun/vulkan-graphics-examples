#include "sample/window.hpp"

#define MODEL_PATH "models/chalet.obj"
#define TEXTURE_PATH "textures/chalet.jpg"

namespace app
{
	SubWindow::SubWindow(uint32_t width, uint32_t height, const char* title,
		std::shared_ptr<vk::Instance> pInstance, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
		std::shared_ptr<vk::Device> device, vk::Queue graphicsQueue, vk::Queue presentQueue)
		: gfw::Window(width, height, title, pInstance, pPhysicalDevice, device, graphicsQueue, presentQueue)
	{
	}

	SubWindow::SubWindow(std::shared_ptr<GLFWwindow> pWindow, std::shared_ptr<vk::SurfaceKHR> pSurface,
		std::shared_ptr<vk::Instance> pInstance, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
		std::shared_ptr<vk::Device> pDevice, vk::Queue graphicsQueue, vk::Queue presentQueue)
		: gfw::Window(pWindow, pSurface, pInstance, pPhysicalDevice, pDevice, graphicsQueue, presentQueue)
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

	Window::Window(uint32_t width, uint32_t height, const char* title,
		std::shared_ptr<vk::Instance> pInstance, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
		std::shared_ptr<vk::Device> device, vk::Queue graphicsQueue, vk::Queue presentQueue)
		: gfw::Window(width, height, title, pInstance, pPhysicalDevice, device, graphicsQueue, presentQueue)
	{
		_loadModel();
		_createMesh();
		_createTexture();
		_createMaterial();
		_createModel();
		_createCamera();
		_createScene();
	}

	Window::Window(std::shared_ptr<GLFWwindow> pWindow, std::shared_ptr<vk::SurfaceKHR> pSurface,
		std::shared_ptr<vk::Instance> pInstance, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
		std::shared_ptr<vk::Device> pDevice, vk::Queue graphicsQueue, vk::Queue presentQueue)
		: gfw::Window(pWindow, pSurface, pInstance, pPhysicalDevice, pDevice, graphicsQueue, presentQueue)
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
			throw std::runtime_error("failed to load texture image!");
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
		m_pPass = std::shared_ptr<kgs::Pass>(new kgs::Pass());
		m_pMaterial = std::shared_ptr<kgs::Material>(new kgs::Material());
		m_pMaterial->setPass("pass0", m_pPass);
		m_pMaterial->setRenderPriority(0u);
		m_pMaterial->setRenderQueueType(kgs::MaterialShowType::OPAQUE);
		m_pPass->setMainTexture(m_pTexture);
		_updateUniform();

	}

	void Window::_createModel()
	{

	}

	void Window::_createCamera()
	{

	}

	void Window::_createScene()
	{

	}

	void Window::_updateUniform()
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		auto duration = currentTime - startTime;
		float time = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() / 1000.0f;

		m_pPass->setData<kgs::MaterialData::DataType::MATRIX>("model"
			, glm::rotate(glm::mat4(), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f))
			, 4u
			, kgs::DescriptorType::UNIFORM_BUFFER
			, kgs::ShaderStageFlagBits::VERTEX
			);

		m_pPass->setData<kgs::MaterialData::DataType::MATRIX>("view"
			, glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0, 0.0f, 0.0f), glm::vec3(0.0, 0.0f, 1.0f))
			, 5u
			, kgs::DescriptorType::UNIFORM_BUFFER
			, kgs::ShaderStageFlagBits::VERTEX
			);

		/*m_pPass->setData<kgs::MaterialData::DataType::MATRIX>("proj"
			, glm::perspective(glm::radians(45.0f),  / (float)(swapChainExtent.height), 0.1f, 10.0f)
			, 6u
			, kgs::DescriptorType::UNIFORM_BUFFER
			, kgs::ShaderStageFlagBits::VERTEX
			);*/
		m_pPass->apply();
	}

	void Window::_onPreUpdate()
	{

	}

	void Window::_update()
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
}