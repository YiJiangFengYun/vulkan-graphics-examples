#ifndef VG_APP_H
#define VG_APP_H

#include <set>
#include "graphics/global.hpp"
#include "graphics/util/vulkan_ext.hpp"
#include "graphics/util/queue_family.hpp"
#include "graphics/util/swapchain_info.hpp"
#include "graphics/util/queue_master.hpp"
#include "graphics/util/util.hpp"

#define VG_ENGINE_NAME "iridescent cloud"
#define VG_VK_API_VERSION VK_API_VERSION_1_0
#define VG_GET_VERSION_MAJOR(version) VK_VERSION_MAJOR(version)
#define VG_GET_VERSION_MINOR(version) VK_VERSION_MINOR(version)
#define VG_GET_VERSION_PATCH(version) VK_VERSION_PATCH(version)

namespace vg
{
#ifdef ENABLE_VALIDATION_LAYERS
	const std::vector<const char*> validationlayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};
#endif // ENABLE_VALIDATION_LAYERS

	const std::vector<const char*> deviceExtensionNames = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	bool checkDeviceExtensionSupport(const PhysicalDevice& physicalDevice, std::vector<const char*> deviceExtensionNames);

	uint32_t countPhysicalDeviceScore(const PhysicalDevice& physicalDevice, const PhysicalDeviceFeaturePriorities &featurePriorities);
	void getRequiredPhysicalDeviceFeaturesFromOptioal(const PhysicalDevice& physicalDevice, 
	    const PhysicalDeviceFeaturePriorities &optionalPhysicalDeviceFeatures,
		PhysicalDeviceFeatures &resultFeatures);

	class Application : public Base
	{
	public:
		Application(std::string name, uint32_t version);
		~Application();

		void initCreateVkInstance(std::vector<const char*> extensions);
		void initOther(const Surface *pSurface
			, uint32_t graphicsQueueCount
			, uint32_t presentQueueCount
			, PhysicalDeviceFeatures requiredPhysicalDeviceFeatures
			, PhysicalDeviceFeaturePriorities optionalPhysicalDeviceFeatures
			);

		//gettor methods

        std::string getAppName() const;
		uint32_t getAppVersion() const;
		std::string getEngineName() const;
		uint32_t getEngineVersion() const;

		vk::Instance *getVKInstance() const;
		vk::PhysicalDevice *getPhysicalDevice() const;
		vk::PhysicalDeviceFeatures &getPhysicalDeviceFeatures() const;
		vk::Device *getDevice() const;
		uint32_t getGraphicsFamily() const;
		uint32_t getPresentFamily() const;
		QueueMaster *getQueueMaster() const;
		vk::CommandPool *getCommandPoolForTransientBuffer() const;
		vk::CommandPool *getCommandPoolForResetBuffer() const;

		void allocateGaphicsQueue(uint32_t &queueIndex, Queue &queue);
		void allocatePresentQueue(uint32_t &queueIndex, Queue &queue);
		void freeGraphicsQueue(uint32_t queueIndex);
		void freePresentQueue(uint32_t queueIndex);
	private:

		Application() = delete;

		std::string m_appName;
		uint32_t m_appVersion;
		std::string m_engineName;
		uint32_t m_engineVersion;
		std::shared_ptr<vk::Instance> m_pInstance;
		vk::PhysicalDeviceFeatures m_physicalDeviceFeatures;
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
		void _pickPhysicalDevice(const vk::SurfaceKHR *pSurface
			, PhysicalDeviceFeatures requiredPhysicalDeviceFeatures
			, PhysicalDeviceFeaturePriorities optionalPhysicalDeviceFeatures
		);
		void _createLogicDevice(const vk::SurfaceKHR *pSurface
			, uint32_t graphicsQueueCount
			, uint32_t presentQueueCount
		);
		void _createCommandPool();
	};

} //namespace kgs

#include "graphics/app/app.inl"

#endif // !VG_APP_H
