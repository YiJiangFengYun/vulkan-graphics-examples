#include "sample/window.hpp"

#define MODEL_PATH "models/chalet.obj"

namespace app
{
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
	}

	Window::Window(std::shared_ptr<GLFWwindow> pWindow, std::shared_ptr<vk::SurfaceKHR> pSurface,
		std::shared_ptr<vk::Instance> pInstance, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
		std::shared_ptr<vk::Device> pDevice, vk::Queue graphicsQueue, vk::Queue presentQueue)
		: gfw::Window(pWindow, pSurface, pInstance, pPhysicalDevice, pDevice, graphicsQueue, presentQueue)
	{
		_loadModel();
		_createMesh();
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
		m_pMesh->setUVs<kgs::UVType::VECTOR_2, kgs::UVIndex::UV_0> (m_tempTexCoords);
		m_pMesh->setIndices(m_tempIndices, kgs::PrimitiveTopology::TRIANGLE_LIST, 0u);
		m_pMesh->apply(KGS_TRUE);
	}

	void Window::_createTexture()
	{

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