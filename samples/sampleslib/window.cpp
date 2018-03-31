#include "sampleslib/window.hpp"

namespace sampleslib
{
	template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_initInputHanders()
	{	
	}

    template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_updateCamera()
	{	
	}

	template<>
	void Window<vg::SpaceType::SPACE_3>::_initInputHanders()
	{
		glfwSetScrollCallback(m_pWindow.get(), [](GLFWwindow *window, double xOffset, double yOffset)
		{
			Window* const instance = (Window*)glfwGetWindowUserPointer(window);
			instance->m_zoom += static_cast<float>(yOffset) * instance->m_zoomSpeed;
			instance->m_zoom = std::min(-0.15f, std::max(-25.0f, instance->m_zoom));
			std::cout << "Current zoom: " << instance->m_zoom << std::endl;
		});

		glfwSetMouseButtonCallback(m_pWindow.get(), [](GLFWwindow* glfwWindow, int button, int action, int /*mods*/) 
		{
			Window* const instance = (Window*)glfwGetWindowUserPointer(glfwWindow);
			if (action == GLFW_PRESS && button >= 0 && button < 3)
			{
				const vgf::Bool32 VALUE = VGF_TRUE;
				memcpy(reinterpret_cast<vgf::Bool32 *>(&(instance->m_mouseButtons)) + button,
					&VALUE, sizeof(vgf::Bool32));
			}
			if (instance->m_mouseButtons.left) 
			{
				glfwGetCursorPos(glfwWindow, instance->m_mousePos, instance->m_mousePos + 1);
			}
		});

		glfwSetCursorPosCallback(m_pWindow.get(), [](GLFWwindow *glfwWindow, double xPos, double yPos)
		{
			bool handledByIM = false;
			ImGuiIO& io = ImGui::GetIO();
		    handledByIM = io.WantCaptureMouse;
			Window* const instance = (Window*)glfwGetWindowUserPointer(glfwWindow);
			auto &lastPos = instance->m_mousePos;
			float dx = static_cast<float>(xPos - lastPos[0]);
			float dy = static_cast<float>(yPos - lastPos[1]);

			lastPos[0] = xPos;
			lastPos[1] = yPos;

            if (handledByIM) return;
			if (instance->m_mouseButtons.left) {
			    instance->m_rotation.x += dy * instance->m_rotationSpeed;
		        instance->m_rotation.y -= dx * instance->m_rotationSpeed;
			}

			if (instance->m_mouseButtons.right) {
				auto &pScenes = instance->m_pScenes;
				uint32_t sceneCount = static_cast<uint32_t>(pScenes.size());
				for (uint32_t i = 0; i < sceneCount; ++i)
				{
					auto &pScene = pScenes[i];
					auto pRootTransform = pScene->pRootTransformForVisualObject;
				    auto localRotation = pRootTransform->getLocalRotation();
				    vg::Vector3 eulerAngles = vg::Vector3(dy * instance->m_rotationSpeed, dx * instance->m_rotationSpeed, 0.0f);
			        vg::Quaternion change = vg::Quaternion(eulerAngles);
				    localRotation = change * localRotation;
				    pRootTransform->setLocalRotation(localRotation);
				    pRootTransform = pScene->pRootTransformForLight;
				    pRootTransform->setLocalRotation(localRotation);
				}
				
			}
		});
	}

	template<>
	void Window<vg::SpaceType::SPACE_2>::_initInputHanders()
	{
	}

    template <>
	void Window<vg::SpaceType::SPACE_3>::_updateCamera()
	{
		m_pCamera->updateProj(glm::radians(60.0f), m_cameraAspect, 0.1f, 256.0f);
		auto pTransform = m_pCamera->getTransform();

		vg::Quaternion eulerAngles = vg::Quaternion(m_rotation);
		pTransform->setLocalPosition(glm::vec3(0.0f, 0.0f, m_zoom));
		pTransform->setLocalRotation(eulerAngles);
		m_pCamera->apply();		
	}

    template <>
	void Window<vg::SpaceType::SPACE_2>::_updateCamera()
	{
		m_pCamera->updateProj(fd::Bounds<glm::vec2>(glm::vec2(-1.0f, -1.0f), 
		    glm::vec2(1.0f, 1.0f)));
		auto pTransform = m_pCamera->getTransform();

		pTransform->setLocalRotation(m_rotation);
		m_pCamera->apply();		
	}
}