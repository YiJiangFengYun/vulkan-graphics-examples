#ifndef GFW_APP_BASE_H
#define GFW_APP_BASE_H

#include <memory>
#include <mutex>
#include "framework/global.hpp"
#include <GLFW/glfw3.h>
#include <foundation/foundation.hpp>
#include "framework/app/window.hpp"
#include "framework/util/thread_master.hpp"

namespace gfw
{
#ifdef ENABLE_VALIDATION_LAYERS
	const std::vector<const char*> validationlayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};
#endif // ENABLE_VALIDATION_LAYERS

	const std::vector<const char*> deviceExtensionNames = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	bool checkDeviceExtensionSupport(const vk::PhysicalDevice& physicalDevice, std::vector<const char*> deviceExtensionNames);

	class App
	{
	public:
		App();
		~App();
		void virtual run();
		template<typename MainWindow_T>
		void init(uint32_t width, uint32_t height, const char *title);

		template<typename Window_T>
		void createSubWindow(uint32_t width, uint32_t height, const char *title);

		//gettor methods
		std::shared_ptr<vk::Instance> getVKInstance();
		std::shared_ptr<vk::PhysicalDevice> getPhysicalDevice();
		std::shared_ptr<vk::Device> getDevice();
		vk::Queue getGraphicsQueue();
		vk::Queue getPresentQueue();

	protected:
		App(const App&) = delete;

		const char* m_appName;
		uint32_t m_appVersion;
		const char* m_engineName;
		uint32_t m_engineVersion;
		std::shared_ptr<vk::Instance> m_pInstance;
		std::shared_ptr<vk::PhysicalDevice> m_pPhysicalDevice;
		std::shared_ptr<vk::Device> m_pDevice;
		vk::Queue m_graphicsQueue;
		vk::Queue m_presentQueue;
		uint32_t m_width;
		uint32_t m_height;
		const char *m_title;
		std::shared_ptr<gfw::Window> m_pWindow;
		std::vector<std::shared_ptr<gfw::Window>> m_pSubWindows;

		//threads
		//ThreadMaster m_threadMaster; 
		//std::mutex m_subWindowsMutex;

		void _initEnv(uint32_t width
			, uint32_t height
			, const char *title
			, std::shared_ptr<GLFWwindow> &pResultGLFWWindow
			, std::shared_ptr<vk::SurfaceKHR> &pResultSurface);
#ifdef ENABLE_VALIDATION_LAYERS
		bool _checkValidationLayerSupport();
#endif // ENABLE_VALIDATION_LAYERS
		void _pickPhysicalDevice(std::shared_ptr<vk::SurfaceKHR> psurface);
		void _createLogicDevice(std::shared_ptr<vk::SurfaceKHR> pSurface);
		void _initGrahpicsModule(std::shared_ptr<vk::SurfaceKHR> pSurface);

		template <typename MainWindow_T>
		void _createWindow(std::shared_ptr<GLFWwindow> pWindow, std::shared_ptr<vk::SurfaceKHR> pSurface);

		//tool methods.
		std::shared_ptr<GLFWwindow> _createGLFWWindow(uint32_t width, uint32_t height, const char* title);
		std::shared_ptr<vk::SurfaceKHR> _createVKSurface(std::shared_ptr<GLFWwindow> pWindow);
		std::vector<const char*> _getRequiredExtensions();
#ifdef DEBUG
		vk::DebugReportCallbackEXT m_debugReportCallBack;
		void _setupDebugCallBack();
		void _onDebugCallBack(vk::DebugReportFlagsEXT flags, vk::DebugReportObjectTypeEXT objType,
			uint64_t obj, size_t location, int32_t code, const char* layerPrefix,
			const char* msg);
		friend VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugReportFlagsEXT flags,
			VkDebugReportObjectTypeEXT objType,
			uint64_t obj,
			size_t location,
			int32_t code,
			const char* layerPrefix,
			const char* msg,
			void* userData);
#endif // DEBUG
	};
}

#include "framework/app/app.inl"

#endif // !GFW_APP_BASE_H
