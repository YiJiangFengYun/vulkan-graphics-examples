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
            instance->m_cameraZoom += static_cast<float>(yOffset) * instance->m_cameraZoomSpeed;
            instance->m_cameraZoom = std::min(-0.15f, instance->m_cameraZoom);
            std::cout << "Current camera zoom: " << instance->m_cameraZoom << std::endl;
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
#ifdef USE_IMGUI_BIND
            ImGuiIO& io = ImGui::GetIO();
            handledByIM = io.WantCaptureMouse;
#endif //USE_IMGUI_BIND
            Window* const instance = (Window*)glfwGetWindowUserPointer(glfwWindow);
            auto &lastPos = instance->m_mousePos;
            float dx = static_cast<float>(xPos - lastPos[0]);
            float dy = static_cast<float>(yPos - lastPos[1]);

            lastPos[0] = xPos;
            lastPos[1] = yPos;

            if (handledByIM) return;
            if (instance->m_mouseButtons.left) {
                instance->m_cameraRotation.x += dy * instance->m_rotationSpeed;
                instance->m_cameraRotation.y += dx * instance->m_rotationSpeed;
            }

            if (instance->m_mouseButtons.right) {
                instance->m_worldRotation.x += dy * instance->m_rotationSpeed;
                instance->m_worldRotation.y += dx * instance->m_rotationSpeed;
                // auto &pScenes = instance->m_pScenes;
                // uint32_t sceneCount = static_cast<uint32_t>(pScenes.size());
                // for (uint32_t i = 0; i < sceneCount; ++i)
                // {
                //     auto &pScene = pScenes[i];
                //     auto pRootTransform = pScene->pRootTransformForVisualObject;
                //     auto localRotation = pRootTransform->getLocalRotation();
                //     vg::Vector3 eulerAngles = vg::Vector3(dy * instance->m_rotationSpeed, dx * instance->m_rotationSpeed, 0.0f);
                //     vg::Quaternion change = vg::Quaternion(eulerAngles);
                //     localRotation = change * localRotation;
                //     pRootTransform->setLocalRotation(localRotation);
                //     pRootTransform = pScene->pRootTransformForLight;
                //     pRootTransform->setLocalRotation(localRotation);
                // }
                
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
        m_pCamera->updateProj(glm::radians(60.0f), m_cameraAspect, 1.0f, 256.0f);

        auto matrix = glm::mat4(1.0f);
        matrix = glm::translate(matrix, glm::vec3(0.0f, 0.0f, m_cameraZoom));

        auto rotationMatrix = glm::mat4(1.0f);
        rotationMatrix = glm::rotate(rotationMatrix, m_cameraRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        rotationMatrix = glm::rotate(rotationMatrix, m_cameraRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        rotationMatrix = glm::rotate(rotationMatrix, m_cameraRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

        auto lookAtRotationMatrix = vg::Matrix4x4(1.0f);
        auto epsilon = std::numeric_limits<float>::epsilon();
        if (glm::length(m_cameraPosition) > epsilon)
        {
            lookAtRotationMatrix = glm::toMat4(glm::rotation(vg::Vector3(0.0f, 0.0f, 1.0f), glm::normalize(-m_cameraPosition)));
        }
        auto translateMatrix = glm::translate(glm::mat4(1.0f), m_cameraPosition);

        auto worldRotationMatrix = glm::mat4(1.0f);
        worldRotationMatrix = glm::rotate(worldRotationMatrix, m_worldRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        worldRotationMatrix = glm::rotate(worldRotationMatrix, m_worldRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        worldRotationMatrix = glm::rotate(worldRotationMatrix, m_worldRotation.z, glm::vec3(0.0f, 1.0f, 0.0f));

        vg::Matrix4x4 localMatrix = worldRotationMatrix * translateMatrix * lookAtRotationMatrix * rotationMatrix * matrix;
        
        // auto lookAtMatrix = glm::lookAt(m_cameraPosition, vg::Vector3(0.0f), vg::Vector3(0.0f, 1.0f, 0.0f));
        // auto localMatrix = lookAtMatrix * rotationMatrix * matrix;

        auto pTransform = m_pCamera->getTransform();

        m_pCamera->getTransform()->setLocalMatrix(localMatrix);

    }

    template <>
    void Window<vg::SpaceType::SPACE_2>::_updateCamera()
    {
        m_pCamera->updateProj(fd::Bounds<glm::vec2>(glm::vec2(-1.0f, -1.0f), 
            glm::vec2(1.0f, 1.0f)));
        auto pTransform = m_pCamera->getTransform();

        pTransform->setLocalPosition(m_cameraPosition);
        pTransform->setLocalRotation(m_cameraRotation);
    }
}