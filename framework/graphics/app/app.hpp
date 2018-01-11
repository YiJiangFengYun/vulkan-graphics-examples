#ifndef KGS_APP_H
#define KGS_APP_H

#include <set>
#include "graphics/global.hpp"
#include "graphics/util/vulkan_ext.hpp"
#include "graphics/util/queue_family.hpp"
#include "graphics/util/swapchain_info.hpp"
#include "graphics/util/queue_master.hpp"

#define ENGINE_NAME "iridescent cloud"
#define ENGINE_VERSION VK_MAKE_VERSION(0, 0, 1)

namespace kgs
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

	class Application : public Base
	{
	public:
		Application(std::string name, uint32_t version);
		~Application();

		void initCreateVkInstance();
		void initOther(std::shared_ptr<vk::SurfaceKHR> pSurface
			, uint32_t graphicsQueueCount
			, uint32_t presentQueueCount
			);

		//gettor methods
		std::shared_ptr<vk::Instance> getVKInstance();
		std::shared_ptr<vk::PhysicalDevice> getPhysicalDevice();
		std::shared_ptr<vk::Device> getDevice();
		uint32_t getGraphicsFamily();
		uint32_t getPresentFamily();
		std::shared_ptr<QueueMaster> getQueueMaster();
		std::shared_ptr<vk::CommandPool> getCommandPoolForTransientBuffer();
		std::shared_ptr<vk::CommandPool> getCommandPoolForResetBuffer();

		void allocateGaphicsQueue(uint32_t &queueIndex, vk::Queue &queue);
		void allocatePresentQueue(uint32_t &queueIndex, vk::Queue &queue);
		void freeGraphicsQueue(uint32_t queueIndex);
		void freePresentQueue(uint32_t queueIndex);
	private:

		Application() = delete;

		std::string m_appName;
		uint32_t m_appVersion;
		const char* m_engineName;
		uint32_t m_engineVersion;
		std::shared_ptr<vk::Instance> m_pInstance;
		std::shared_ptr<vk::PhysicalDevice> m_pPhysicalDevice;
		std::shared_ptr<vk::Device> m_pDevice;
		uint32_t m_graphicsFamily;
		uint32_t m_presentFamily;
		std::shared_ptr<QueueMaster> m_pQueueMaster;
		std::shared_ptr<vk::CommandPool> m_pCommandPoolForTransientBuffer;
		std::shared_ptr<vk::CommandPool> m_pCommandPoolForResetBuffer;


#ifdef ENABLE_VALIDATION_LAYERS
		bool _checkValidationLayerSupport();
#endif // ENABLE_VALIDATION_LAYERS

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
		std::vector<const char*> _getRequiredExtensions();
		void _pickPhysicalDevice(std::shared_ptr<vk::SurfaceKHR> psurface);
		void _createLogicDevice(std::shared_ptr<vk::SurfaceKHR> pSurface
			, uint32_t graphicsQueueCount
			, uint32_t presentQueueCount
		);
		void _createCommandPool();
	};

} //namespace kgs

#endif // !KGS_APP_H
