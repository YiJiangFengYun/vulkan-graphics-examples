#include "framework/app/app.hpp"

#include <plog/Log.h>
#include <plog/Appenders/DebugOutputAppender.h>
#include <set>
#include <thread>

namespace vgf {

    void runWindow(std::shared_ptr<Window> pSubWindow)
    {
        pSubWindow->run();
    }

    App::App()
        : m_width(0u)
        , m_height(0u)
        , m_title("")
        , m_graphicsQueueCount(DEFAULT_GRAPHICS_QUEUE_COUNT)
        , m_presentQueuecount(DEFAULT_PRESENT_QUEUE_COUNT)
    {

    }

    App::~App()
    {
        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        //Initialize vgim module
#ifdef USE_IMGUI_BIND
        vgim::moduleDestory();
#endif //USE_IMGUI_BIND
        //-------------------------------------------------------------

        m_pSubWindows.resize(0);
        m_pWindow = nullptr;

        vg::moduleDestory();

        glfwTerminate();
    }

    void App::run()
    {
        //std::thread thread_test;

        while (m_pWindow->windowShouldClose() == VGF_FALSE)
        {
            glfwPollEvents();            
            //remove closed child(sub) windows.
            m_pSubWindows.erase(std::remove_if(m_pSubWindows.begin(), m_pSubWindows.end(), [](const std::shared_ptr<Window>& item) {
                return item->windowShouldClose();
            }), m_pSubWindows.end());

            ////multiply threads.
            //ThreadMaster threadMaster;
            //threadMaster.appendThread(std::shared_ptr<std::thread>(new std::thread(runWindow, m_pWindow)));
            //for (const auto& pSubWindow : m_pSubWindows)
            //{
            //    threadMaster.appendThread(std::shared_ptr<std::thread>(new std::thread(runWindow, pSubWindow)));
            //};
            //threadMaster.join();

            //single thread.
            m_pWindow->run();
            for (const auto& pSubWindow : m_pSubWindows)
            {
                pSubWindow->run();
            };
            vg::pApp->getDevice()->waitIdle();
        }
    }

    void App::_initEnv(uint32_t width
        , uint32_t height
        , const char *title
        , std::shared_ptr<GLFWwindow> &pResultGLFWWindow
        , std::shared_ptr<vk::SurfaceKHR> &pResultSurface
        , vg::PhysicalDeviceFeatures requiredPhysicalDeviceFeatures
        , vg::PhysicalDeviceFeaturePriorities optionalPhysicalDeviceFeatures
        )
    {
        m_width = width;
        m_height = height;
        m_title = title;

        //----------------------------------------------------------------------
        //Initialize glfw module.
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        //Initialize vg module.                

        uint32_t glfwExtensionCount;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char*> vkExtensions(glfwExtensionCount);
        uint32_t i = 0;
        for (i = 0; i < glfwExtensionCount; ++i)
        {
            vkExtensions[i] = glfwExtensions[i];
        }
        vg::moduleCreateVkinstance("vulkan graphics"
            , VK_MAKE_VERSION(1, 0, 0)
            , vkExtensions);

        pResultGLFWWindow = _createGLFWWindow(m_width, m_height, m_title);
        pResultSurface = _createVKSurface(pResultGLFWWindow.get());

        vg::moduleCreateOther(pResultSurface
            , m_graphicsQueueCount
            , m_presentQueuecount
            , requiredPhysicalDeviceFeatures
            , optionalPhysicalDeviceFeatures
            );

        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        //Initialize vgim module
#ifdef USE_IMGUI_BIND
        vgim::moduleCreateCanvas(m_width, m_height);
#endif //USE_IMGUI_BIND
        //----------------------------------------------------------------------                
        
    }

    std::shared_ptr<GLFWwindow> App::_createGLFWWindow(uint32_t width, uint32_t height, const char* title)
    {
        return createGLFWWindow(width, height, title);
    }

    std::shared_ptr<vk::SurfaceKHR> App::_createVKSurface(GLFWwindow *pWindow)
    {
        auto pInstance = vg::pApp->getVKInstance();
        return createSurface(pInstance, pWindow);
    }
}
