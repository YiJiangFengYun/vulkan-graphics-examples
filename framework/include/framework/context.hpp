#ifndef FW_CONTEXT_H
#define FW_CONTEXT_H

#include <vector>
#include <vulkan/vulkan.hpp>
#include "macro.hpp"
#include "vulkanExt.hpp"
#include <GLFW/glfw3.h>

namespace fw
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

	bool checkDeviceExtensionSupport(vk::PhysicalDevice physicalDevice, std::vector<const char*> deviceExtensionNames);

	struct UsedQueueFamily
	{
		uint32_t graphicsFamily = -1;
		uint32_t presentFamily = -1;

		Bool32 isComplete()
		{
			return graphicsFamily >= 0 && presentFamily >= 0;
		}

		UsedQueueFamily static findQueueFamilies(vk::PhysicalDevice physicalDevice, VkSurfaceKHR surface);
	};

	struct SwapChainSupportDetails {
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
		SwapChainSupportDetails static querySwapChainSupport(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
	};

	class Context
	{
	public:
		Context(const char* appName, uint32_t appVersion, const char* engineName, uint32_t engineVersion);
		~Context();
		void init(GLFWwindow* pWindow);
	private:
		const char* m_appName;
		uint32_t m_appVersion;
		const char* m_engineName;
		uint32_t m_engineVersion;
		vk::Instance m_instance;
		vk::SurfaceKHR m_surface;
		vk::PhysicalDevice m_physicalDevice;
		vk::Device m_device;
		bool _checkValidationLayerSupport();
		void _createSurface(GLFWwindow* window);
		void _pickPhysicalDevice();
		void _createLogicDevice();
		// tool methods
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



#endif // !FW_CONTEXT_H

