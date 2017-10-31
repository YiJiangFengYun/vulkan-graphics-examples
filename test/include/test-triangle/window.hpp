#ifndef WINDOW_H
#define WINDOW_H

#include "testlib/stb_image.h"
#include "testlib/tiny_obj_loader.h"
#include "framework/framework.hpp"

namespace testTriangle
{
	class SubWindow : public gfw::Window
	{
	public:
		SubWindow(uint32_t width
			, uint32_t height
			, const char* title
			, std::shared_ptr<vk::Instance> pInstance
			, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
			, std::shared_ptr<vk::Device> pDevice
			, vk::Queue graphicsQueue
			, vk::Queue presentQueue
		);

		SubWindow(uint32_t width
			, uint32_t height
			, const char* title
			, RenderType renderType
			, std::shared_ptr<vk::Instance> pInstance
			, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
			, std::shared_ptr<vk::Device> pDevice
			, vk::Queue graphicsQueue
			, vk::Queue presentQueue
		);

		SubWindow(std::shared_ptr<GLFWwindow> pWindow
			, std::shared_ptr<vk::SurfaceKHR> pSurface
			, std::shared_ptr<vk::Instance> pInstance
			, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
			, std::shared_ptr<vk::Device> pDevice
			, vk::Queue graphicsQueue
			, vk::Queue presentQueue
		);

		SubWindow(RenderType renderType
			, std::shared_ptr<GLFWwindow> pWindow
			, std::shared_ptr<vk::SurfaceKHR> pSurface
			, std::shared_ptr<vk::Instance> pInstance
			, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
			, std::shared_ptr<vk::Device> pDevice
			, vk::Queue graphicsQueue
			, vk::Queue presentQueue
		);

	private:
		void _onPreUpdate() override;
		void _update() override;
		void _onPostUpdate() override;

		void _onPreRender() override;
		void _onPostRender() override;
	};

	class Window : public gfw::Window
	{
	public:
		struct Vertex
		{
			glm::vec3 pos;
			glm::vec3 color;
			glm::vec2 texCoord;

			bool operator==(const Vertex& other) const;
		};

		Window(uint32_t width
			, uint32_t height
			, const char* title
			, std::shared_ptr<vk::Instance> pInstance
			, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
			, std::shared_ptr<vk::Device> pDevice
			, vk::Queue graphicsQueue
			, vk::Queue presentQueue
		);

		Window(uint32_t width
			, uint32_t height
			, const char* title
			, RenderType renderType
			, std::shared_ptr<vk::Instance> pInstance
			, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
			, std::shared_ptr<vk::Device> pDevice
			, vk::Queue graphicsQueue
			, vk::Queue presentQueue
		);

		Window(std::shared_ptr<GLFWwindow> pWindow
			, std::shared_ptr<vk::SurfaceKHR> pSurface
			, std::shared_ptr<vk::Instance> pInstance
			, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
			, std::shared_ptr<vk::Device> pDevice
			, vk::Queue graphicsQueue
			, vk::Queue presentQueue
		);

		Window(RenderType renderType
			, std::shared_ptr<GLFWwindow> pWindow
			, std::shared_ptr<vk::SurfaceKHR> pSurface
			, std::shared_ptr<vk::Instance> pInstance
			, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
			, std::shared_ptr<vk::Device> pDevice
			, vk::Queue graphicsQueue
			, vk::Queue presentQueue
		);

	private:
		std::vector<kgs::Vector3> m_tempPositions;
		std::vector<kgs::Color32> m_tempColors;
		std::vector<uint32_t> m_tempIndices;


		std::shared_ptr<kgs::Scene3> m_pScene;
		std::shared_ptr<kgs::Camera3> m_pCamera;
		std::shared_ptr<kgs::VisualObject3> m_pModel;
		std::shared_ptr<kgs::Mesh3> m_pMesh;
		std::shared_ptr<kgs::Material> m_pMaterial;
		std::shared_ptr<kgs::Pass> m_pPass;
		std::shared_ptr<kgs::Shader> m_pShader;


		void _loadModel();
		void _createMesh();
		void _createMaterial();
		void _createModel();
		void _createCamera();
		void _createScene();
		void _fillRenderer();

		void _onPreUpdate() override;
		void _update() override;
		void _onPostUpdate() override;

		void _onPreRender() override;
	    void _onPostRender() override;
	};

}

namespace std {
	template<> 
	struct hash<testTriangle::Window::Vertex> {
		size_t operator()(const testTriangle::Window::Vertex& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}

#endif // !WINDOW_H
