#ifndef GFW_APP_BASE_H
#define GFW_APP_BASE_H

#include <memory>
#include "macro.hpp"
#include "window.hpp"
#include <GLFW/glfw3.h>

namespace gfw
{
#ifdef DEBUG
#define ENABLE_VALIDATION_LAYERS
	const std::vector<const char*> validationlayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};
#endif // DEBUG

	const std::vector<const char*> deviceExtensionNames = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	bool checkDeviceExtensionSupport(const vk::PhysicalDevice& physicalDevice, std::vector<const char*> deviceExtensionNames);

	class AppBase
	{
	public:
		AppBase(uint32_t width, uint32_t height, const char *title);
		~AppBase();
		void virtual run();
		void createSubWindow(uint32_t width, uint32_t height, const char *title);

		//gettor methods
		vk::Instance getVKInstance();
		vk::PhysicalDevice getPhysicalDevice();
		vk::Device getDevice();
		vk::Queue getGraphicsQueue();
		vk::Queue getPresentQueue();
	protected:

	private:
		AppBase(const AppBase&);

		const char* m_appName;
		uint32_t m_appVersion;
		const char* m_engineName;
		uint32_t m_engineVersion;
		vk::Instance m_instance;
		vk::PhysicalDevice m_physicalDevice;
		vk::Device m_device;
		vk::Queue m_graphicsQueue;
		vk::Queue m_presentQueue;
		int32_t m_width;
		int32_t m_height;
		const char *m_title;
		std::shared_ptr<gfw::Window> m_pWindow;
		std::vector<std::shared_ptr<gfw::Window>> m_pSubWindows;

		void _init();
		bool _checkValidationLayerSupport();
		void _pickPhysicalDevice(vk::SurfaceKHR surface);
		void _createLogicDevice(vk::SurfaceKHR surface);
		void _createWindow(GLFWwindow *pwindow, vk::SurfaceKHR surface);
		std::shared_ptr<Window> _createSubWindow(uint32_t width, uint32_t height, const char* title);

		//tool methods.
		GLFWwindow* _createGLFWWindow(uint32_t width, uint32_t height, const char* title);
		vk::SurfaceKHR _createVKSurface(GLFWwindow* pWindow);
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

#endif // !GFW_APP_BASE_H
