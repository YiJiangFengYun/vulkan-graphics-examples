#include "graphics/app/app.hpp"

namespace vg
{
	bool checkDeviceExtensionSupport(const vk::PhysicalDevice& physicalDevice, std::vector<const char*> deviceExtensionNames)
	{
		auto extensionProperties = physicalDevice.enumerateDeviceExtensionProperties();

		std::set<std::string> requiredExtensionNames(deviceExtensionNames.begin(), deviceExtensionNames.end());

		for (const auto& extPro : extensionProperties)
		{
			requiredExtensionNames.erase(extPro.extensionName);
		}

		return requiredExtensionNames.empty();
	}

	uint32_t countPhysicalDeviceScore(const PhysicalDevice& physicalDevice, const PhysicalDeviceFeaturePriorities &featurePriorities)
	{
		auto features = physicalDevice.getFeatures();
		uint32_t sizeOfStruct = static_cast<uint32_t>(sizeof(features));
		uint32_t sizeOfItem1 = static_cast<uint32_t>(sizeof(vk::Bool32));
		uint32_t sizeOfItem2 = static_cast<uint32_t>(sizeof(uint32_t));
		uint32_t countOfItem = sizeOfStruct / sizeOfItem1;
		uint32_t score = 0u;
		for (uint32_t i = 0; i < countOfItem; ++i)
		{
			vk::Bool32 valueOfItem1;
			memcpy(&valueOfItem1, reinterpret_cast<char *>(&features) + sizeOfItem1 * i, sizeof(vk::Bool32));
			if (valueOfItem1)
			{
				uint32_t valueOfItem2;
			    memcpy(&valueOfItem2, reinterpret_cast<const char *>(&featurePriorities) + sizeOfItem2 * i, sizeof(uint32_t));
				score += valueOfItem2;
			}
		}
		return score;
	}

	void getRequiredPhysicalDeviceFeaturesFromOptioal(const PhysicalDevice& physicalDevice, 
	    const PhysicalDeviceFeaturePriorities &optionalPhysicalDeviceFeatures,
		PhysicalDeviceFeatures &resultFeatures)
	{
		auto features = physicalDevice.getFeatures();
		uint32_t sizeOfStruct = static_cast<uint32_t>(sizeof(features));
		uint32_t sizeOfItem1 = static_cast<uint32_t>(sizeof(vk::Bool32));
		uint32_t sizeOfItem2 = static_cast<uint32_t>(sizeof(uint32_t));
		uint32_t countOfItem = sizeOfStruct / sizeOfItem1;
		vk::Bool32 open = VK_TRUE;
		for (uint32_t i = 0; i < countOfItem; ++i)
		{
			vk::Bool32 valueOfItem1;
			memcpy(&valueOfItem1, reinterpret_cast<char *>(&features) + sizeOfItem1 * i, sizeof(vk::Bool32));
			if (valueOfItem1)
			{
				uint32_t valueOfItem2;
			    memcpy(&valueOfItem2, reinterpret_cast<const char *>(&optionalPhysicalDeviceFeatures) + sizeOfItem2 * i, sizeof(uint32_t));
				if (valueOfItem2)
				{
					memcpy(reinterpret_cast<char *>(&resultFeatures) + sizeOfItem1 * i, &open, sizeof(vk::Bool32));
				}
			}
		}
	}

	Application::Application(std::string name, uint32_t version)
		: Base(BaseType::APP)
		, m_appName(name)
		, m_appVersion(version)
		, m_engineName(VG_ENGINE_NAME)
		, m_engineVersion()
	{
		m_engineVersion = VK_MAKE_VERSION(std::stoi(VG_VERSION_MAJOR), std::stoi(VG_VERSION_MINOR), std::stoi(VG_VERSION_PATCH));
	}

	Application::~Application()
	{
#ifdef DEBUG
		if (m_debugReportCallBack != vk::DebugReportCallbackEXT(nullptr))
		{
			destroyDebugReportCallbackEXT(static_cast<VkInstance>(*m_pInstance)
				, static_cast<VkDebugReportCallbackEXT>(m_debugReportCallBack), nullptr);
		}
#endif // DEBUG
	}

	void Application::initCreateVkInstance(std::vector<const char*> extensions)
	{
#ifdef ENABLE_VALIDATION_LAYERS
		if (_checkValidationLayerSupport() == false)
		{
			throw std::runtime_error("Validation layers requested is not available!");
		}
#endif // ENABLE_VALIDATION_LAYERS
		vk::ApplicationInfo appInfo = { m_appName.data(), m_appVersion, m_engineName.data(), m_engineVersion, VG_VK_API_VERSION };

		//query application required extensions.
		auto requiredExtensions = _getRequiredExtensions();
		uint32_t requiredExtIndex = requiredExtensions.size();
		requiredExtensions.resize(requiredExtIndex + extensions.size());
		for (const auto& extension : extensions) {
			requiredExtensions[requiredExtIndex] = extension;
			++requiredExtIndex;
		}
#ifdef DEBUG
		//query available extensions.
		auto availableExtensions = vk::enumerateInstanceExtensionProperties();

		//print available extensions.
		VG_LOG(plog::debug) << "Available extensions: " << std::endl;
		for (const auto& extension : availableExtensions)
		{
			VG_LOG(plog::debug) << "\t" << extension.extensionName << std::endl;
		}
#endif // DEBUG

#ifdef ENABLE_VALIDATION_LAYERS
		vk::InstanceCreateInfo createInfo = {
			vk::InstanceCreateFlags(),
			&appInfo,
			static_cast<uint32_t>(validationlayers.size()),
			validationlayers.data(),
			static_cast<uint32_t>(requiredExtensions.size()),
			requiredExtensions.data()
		};
#else
		vk::InstanceCreateInfo createInfo = {
			vk::InstanceCreateFlags(),
			&appInfo,
			0,
			nullptr,
			static_cast<uint32_t>(requiredExtensions.size()),
			requiredExtensions.data()
		};
#endif // ENABLE_VALIDATION_LAYERS

		//create vulkan instance
		m_pInstance = fd::createInstance(createInfo);

#ifdef DEBUG
		_setupDebugCallBack();
#endif // DEBUG
	}

	void Application::initOther(const vk::SurfaceKHR *pSurface
		, uint32_t graphicsQueueCount
		, uint32_t presentQueueCount
		, PhysicalDeviceFeatures requiredPhysicalDeviceFeatures
		, PhysicalDeviceFeaturePriorities optionalPhysicalDeviceFeatures
	)
	{
		_pickPhysicalDevice(pSurface, requiredPhysicalDeviceFeatures, optionalPhysicalDeviceFeatures);
		_createLogicDevice(pSurface
			, graphicsQueueCount
			, presentQueueCount
		);
		_createCommandPool();
	}

	std::string Application::getAppName() const
	{
		return m_appName;
	}
		
	uint32_t Application::getAppVersion() const
	{
		return m_appVersion;
	}

	std::string Application::getEngineName() const
	{
		return m_engineName;
	}
		
	uint32_t Application::getEngineVersion() const
	{
		return m_engineVersion;
	}

	vk::Instance *Application::getVKInstance() const
	{
		return m_pInstance.get();
	}

	vk::PhysicalDevice *Application::getPhysicalDevice() const
	{
		return m_pPhysicalDevice.get();
	}

	vk::PhysicalDeviceFeatures &Application::getPhysicalDeviceFeatures() const
	{
		return m_pPhysicalDevice->getFeatures();
	}

	vk::Device *Application::getDevice() const
	{
		return m_pDevice.get();
	}

	uint32_t Application::getGraphicsFamily() const
	{
		return m_graphicsFamily;
	}

	uint32_t Application::getPresentFamily() const
	{
		return m_presentFamily;
	}

	QueueMaster *Application::getQueueMaster() const
	{
		return m_pQueueMaster.get();
	}

	vk::CommandPool *Application::getCommandPoolForTransientBuffer() const
	{
		return m_pCommandPoolForTransientBuffer.get();
	}

	vk::CommandPool *Application::getCommandPoolForResetBuffer() const
	{
		return m_pCommandPoolForResetBuffer.get();
	}

	void Application::allocateGaphicsQueue(uint32_t &queueIndex, vk::Queue &queue)
	{
		m_pQueueMaster->allocateQueue(m_graphicsFamily, queueIndex, queue);
	}

	void Application::allocatePresentQueue(uint32_t &queueIndex, vk::Queue &queue)
	{
		m_pQueueMaster->allocateQueue(m_presentFamily, queueIndex, queue);
	}

	void Application::freeGraphicsQueue(uint32_t queueIndex)
	{
		m_pQueueMaster->freeQueue(m_graphicsFamily, queueIndex);
	}
	void Application::freePresentQueue(uint32_t queueIndex)
	{
		m_pQueueMaster->freeQueue(m_presentFamily, queueIndex);
	}

#ifdef ENABLE_VALIDATION_LAYERS
	bool Application::_checkValidationLayerSupport()
	{
		//query available layers.
		auto avaibleLayers = vk::enumerateInstanceLayerProperties();

		//print available layers.
		VG_LOG(plog::debug) << "Available layers: " << std::endl;
		for (const auto& layerProperties : avaibleLayers)
		{
			VG_LOG(plog::debug) << "\t" << layerProperties.layerName << std::endl;
		}

		for (const auto& layerName : validationlayers)
		{
			bool layerFound = false;
			for (const auto& layerProperties : avaibleLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (layerFound == false)
			{
				return false;
			}
		}

		return true;
	}
#endif // ENABLE_VALIDATION_LAYERS

	std::vector<const char*> Application::_getRequiredExtensions()
	{
		std::vector<const char*> requiredExtensions;

		uint32_t extensionCount = 0;

#ifdef ENABLE_VALIDATION_LAYERS
		uint32_t requiredExtensionCount = extensionCount + 1;
#else
		uint32_t requiredExtensionCount = extensionCount;
#endif // ENABLE_VALIDATION_LAYERS

		requiredExtensions.resize(requiredExtensionCount);

#ifdef ENABLE_VALIDATION_LAYERS
		requiredExtensions[extensionCount] = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
#endif // ENABLE_VALIDATION_LAYERS

		return requiredExtensions;
	}

#ifdef DEBUG
	void Application::_setupDebugCallBack()
	{
		vk::DebugReportCallbackCreateInfoEXT createInfo = {
			vk::DebugReportFlagBitsEXT::eDebug |
			vk::DebugReportFlagBitsEXT::eInformation |
			vk::DebugReportFlagBitsEXT::eWarning |
			vk::DebugReportFlagBitsEXT::ePerformanceWarning |
			vk::DebugReportFlagBitsEXT::eError,
			debugCallback,
			this
		};

		VkDebugReportCallbackEXT callback;
		createDebugReportCallbackEXT(static_cast<VkInstance>(*m_pInstance), &VkDebugReportCallbackCreateInfoEXT(createInfo),
			nullptr, &callback);
		m_debugReportCallBack = static_cast<vk::DebugReportCallbackEXT>(callback);
	}

	void Application::_onDebugCallBack(vk::DebugReportFlagsEXT flags, vk::DebugReportObjectTypeEXT objType,
		uint64_t obj, size_t location, int32_t code, const char* layerPrefix,
		const char* msg)
	{
		if (flags & vk::DebugReportFlagBitsEXT::eDebug)
		{
			VG_LOG(plog::debug) << code << " : " << msg << " at " << layerPrefix << std::endl;
		}
		else if (flags & vk::DebugReportFlagBitsEXT::eInformation)
		{
			VG_LOG(plog::info) << code << " : " << msg << " at " << layerPrefix << std::endl;
		}
		else if (flags & vk::DebugReportFlagBitsEXT::eWarning)
		{
			VG_LOG(plog::warning) << code << " : " << msg << " at " << layerPrefix << std::endl;
		}
		else if (flags & vk::DebugReportFlagBitsEXT::ePerformanceWarning)
		{
			VG_LOG(plog::warning) << code << " : " << msg << " at " << layerPrefix << std::endl;
		}
		else if (flags & vk::DebugReportFlagBitsEXT::eError)
		{
			VG_LOG(plog::error) << code << " : " << msg << " at " << layerPrefix << std::endl;
			throw std::runtime_error(std::to_string(code) + " : " + std::string(msg) + " at " + std::string(layerPrefix));
		}
		else
		{
			VG_LOG(plog::info) << code << " : " << msg << " at " << layerPrefix << std::endl;
		}
	}
#endif // DEBUG

#ifdef DEBUG
	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t obj,
		size_t location,
		int32_t code,
		const char* layerPrefix,
		const char* msg,
		void * userData)
	{
		Application* context = (Application *)userData;
		context->_onDebugCallBack(vk::DebugReportFlagsEXT(vk::DebugReportFlagBitsEXT(flags)),
			vk::DebugReportObjectTypeEXT(objType),
			obj, location,
			code,
			layerPrefix,
			msg);

		return VK_FALSE;
	}
#endif // DEBUG

	void Application::_pickPhysicalDevice(const vk::SurfaceKHR *pSurface
		, PhysicalDeviceFeatures requiredPhysicalDeviceFeatures
		, PhysicalDeviceFeaturePriorities optionalPhysicalDeviceFeatures
	)
	{
		auto physicalDevices = m_pInstance->enumeratePhysicalDevices();

		VG_LOG(plog::debug) << "physical device num: " + physicalDevices.size() << std::endl;

		if (physicalDevices.size() == 0)
		{
			throw std::runtime_error("Failed to find GPUs with VULKAN support.");
		}

		for (auto it = physicalDevices.cbegin(); it != physicalDevices.cend();)
		{
			Bool32 isSuitable = [&](const vk::PhysicalDevice& physicalDevice)->Bool32
			{
				//const vk::PhysicalDeviceProperties deviceProperties = physicalDevice.getProperties();
				const vk::PhysicalDeviceFeatures deviceFeatures = physicalDevice.getFeatures();
				//Application can't function without need features.
				if (isContainStruct(deviceFeatures, requiredPhysicalDeviceFeatures) == VG_FALSE)
				{
					return VG_FALSE;
				}

				//Application can't function without queue family that supports graphics commands.
				if (UsedQueueFamily::findQueueFamilies(physicalDevice, *pSurface).isComplete() == VG_FALSE)
				{
					return VG_FALSE;
				}

				if (checkDeviceExtensionSupport(physicalDevice, deviceExtensionNames) == VG_FALSE)
				{
					return VG_FALSE;
				}

				//Application can't function without adequate support of device for swap chain.
				SwapChainSupportDetails swapChainSupportDetails = SwapChainSupportDetails::querySwapChainSupport(physicalDevice, *pSurface);
				if (swapChainSupportDetails.formats.empty() || swapChainSupportDetails.presentModes.empty())
				{
					return VG_FALSE;
				}

				return VG_TRUE;
			}(*it);

			if (isSuitable == VG_FALSE)
			{
				physicalDevices.erase(it);
				if (physicalDevices.size() == 0) break;
			}
			else
			{
				++it;
			}
		}

		if (physicalDevices.size() == 0)
		{
			throw std::runtime_error("Failed to find a suitable GPU!");
		}

		m_physicalDeviceFeatures = requiredPhysicalDeviceFeatures;

		std::sort(physicalDevices.begin(), physicalDevices.end(),
			[&](const vk::PhysicalDevice& physicalDevice1, const vk::PhysicalDevice& physicalDevice2) {
			int32_t result = 0;
			const vk::PhysicalDeviceProperties deviceProperties1 = physicalDevice1.getProperties();
			const vk::PhysicalDeviceProperties deviceProperties2 = physicalDevice2.getProperties();
			const vk::PhysicalDeviceFeatures deviceFeatures1 = physicalDevice1.getFeatures();
			const vk::PhysicalDeviceFeatures deviceFeatures2 = physicalDevice2.getFeatures();

			if (result)
			{
			    //Calculate score of the physical device features.
			    uint32_t score1 = countPhysicalDeviceScore(physicalDevice1, optionalPhysicalDeviceFeatures);
			    uint32_t score2 = countPhysicalDeviceScore(physicalDevice2, optionalPhysicalDeviceFeatures);
				result = score1 - score2;
			}

			if (result == 0)
			{
				int32_t value1 = static_cast<int32_t>(deviceProperties1.deviceType == vk::PhysicalDeviceType::eDiscreteGpu);
				int32_t value2 = static_cast<int32_t>(deviceProperties2.deviceType == vk::PhysicalDeviceType::eDiscreteGpu);
				result = value1 - value2;
			}
			if (result == 0)
			{
				result = deviceProperties1.limits.maxImageDimension2D - deviceProperties2.limits.maxImageDimension2D;
			}
			return result >= 0;
		});

		m_pPhysicalDevice = std::shared_ptr<vk::PhysicalDevice>(new vk::PhysicalDevice(*physicalDevices.cbegin()));
		getRequiredPhysicalDeviceFeaturesFromOptioal(*m_pPhysicalDevice, optionalPhysicalDeviceFeatures, m_physicalDeviceFeatures);
		VG_LOG(plog::debug) << "Pick successfully physical device.";
	}

	void Application::_createLogicDevice(const vk::SurfaceKHR *pSurface
		, uint32_t graphicsQueueCount
		, uint32_t presentQueueCount
	)
	{
		UsedQueueFamily usedQueueFamily = UsedQueueFamily::findQueueFamilies(*m_pPhysicalDevice, *pSurface);
		m_graphicsFamily = usedQueueFamily.graphicsFamily;
		m_presentFamily = usedQueueFamily.presentFamily;

		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
		std::unordered_map<uint32_t, uint32_t> mapFamilyAndQueueCounts;

		graphicsQueueCount = std::min(graphicsQueueCount, usedQueueFamily.graphicsMaxQueueCount);
		presentQueueCount = std::min(presentQueueCount, usedQueueFamily.presentMaxQueueCount);

		mapFamilyAndQueueCounts[usedQueueFamily.graphicsFamily] += graphicsQueueCount;
		mapFamilyAndQueueCounts[usedQueueFamily.presentFamily] += presentQueueCount;

		mapFamilyAndQueueCounts[usedQueueFamily.graphicsFamily] =
			std::min(mapFamilyAndQueueCounts[usedQueueFamily.graphicsFamily], usedQueueFamily.graphicsMaxQueueCount);

		mapFamilyAndQueueCounts[usedQueueFamily.presentFamily] =
			std::min(mapFamilyAndQueueCounts[usedQueueFamily.presentFamily], usedQueueFamily.presentMaxQueueCount);

		queueCreateInfos.resize(mapFamilyAndQueueCounts.size());

		graphicsQueueCount = mapFamilyAndQueueCounts[usedQueueFamily.graphicsFamily];
		presentQueueCount = mapFamilyAndQueueCounts[usedQueueFamily.presentFamily];
		
		std::unordered_map<uint32_t, std::vector<float>> familiyPriorites;
		familiyPriorites[usedQueueFamily.graphicsFamily] = std::vector<float>(graphicsQueueCount, 0.0f);
		familiyPriorites[usedQueueFamily.presentFamily] = std::vector<float>(presentQueueCount, 0.0f);

		size_t index = 0;
		for (const auto& item : mapFamilyAndQueueCounts)
		{
			vk::DeviceQueueCreateInfo queueCreateInfo = {
				vk::DeviceQueueCreateFlags(),                 //flags
				static_cast<uint32_t>(item.first),            //queueFamilyIndex
				uint32_t(item.second),                        //queueCount
				familiyPriorites[item.first].data()           //pQueuePriorities
			};
			queueCreateInfos[index] = queueCreateInfo;
			++index;
		}

		vk::DeviceCreateInfo createInfo = {
			vk::DeviceCreateFlags(),                                     //flags
			static_cast<uint32_t>(queueCreateInfos.size()),              //queueCreateInfoCount
			queueCreateInfos.data(),                                     //pQueueCreateInfos
			0,                                                           //enabledLayerCount
			nullptr,                                                     //ppEnabledLayerNames
			static_cast<uint32_t>(deviceExtensionNames.size()),          //enabledExtensionCount
			deviceExtensionNames.data(),                                 //ppEnabledExtensionNames
			&m_physicalDeviceFeatures                                    //pEnabledFeatures
		};

#ifdef ENABLE_VALIDATION_LAYERS
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationlayers.size());
		createInfo.ppEnabledLayerNames = validationlayers.data();
#endif // ENABLE_VALIDATION_LAYERS

		m_pDevice = fd::createDevice(m_pPhysicalDevice.get(), createInfo);
		m_pQueueMaster = std::shared_ptr<QueueMaster>(new QueueMaster(m_pDevice
			, mapFamilyAndQueueCounts
		));

		VG_LOG(plog::debug) << "Create successfully logic device.";
	}

	void Application::_createCommandPool()
	{
		vk::CommandPoolCreateInfo createInfoForTransientBuffer = {
			vk::CommandPoolCreateFlagBits::eTransient,
			m_graphicsFamily
		};
		m_pCommandPoolForTransientBuffer = fd::createCommandPool(m_pDevice.get(), createInfoForTransientBuffer);

		vk::CommandPoolCreateInfo createInfoForResetBuffer = {
			vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
			m_graphicsFamily
		};
		m_pCommandPoolForResetBuffer = fd::createCommandPool(m_pDevice.get(), createInfoForResetBuffer);
	}
} //namespace kgs