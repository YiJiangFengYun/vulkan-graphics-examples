#ifndef WINDOW_H
#define WINDOW_H

#include "framework/app/window.hpp"

namespace app
{
	class Window : public gfw::Window
	{
	public:

		Window(uint32_t width, uint32_t height, const char* title,
			std::shared_ptr<vk::Instance> pInstance, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
			std::shared_ptr<vk::Device> pDevice, vk::Queue graphicsQueue, vk::Queue presentQueue);

		Window(std::shared_ptr<GLFWwindow> pWindow, std::shared_ptr<vk::SurfaceKHR> pSurface,
			std::shared_ptr<vk::Instance> pInstance, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
			std::shared_ptr<vk::Device> pDevice, vk::Queue graphicsQueue, vk::Queue presentQueue);

	private:
		kgs::Scene3 m_scene;
		kgs::Camera3 m_camera;
		kgs::VisualObject3 m_model;
		kgs::Mesh3 m_mesh;
		kgs::Material m_material;
		kgs::Pass m_pass;
		kgs::Shader m_shader;
		kgs::Texture2D m_texture;


		void _loadModel();

		void _onPreUpdate() override;
		void _update() override;
		void _onPostUpdate() override;

		void _onPreRender() override;
	    void _onPostRender() override;
	};

}

#endif // !WINDOW_H
