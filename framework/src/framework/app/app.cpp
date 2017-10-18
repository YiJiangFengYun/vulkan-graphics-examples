#include "framework/app/app.hpp"

#include <plog/Log.h>
#include <plog/Appenders/DebugOutputAppender.h>
#include <set>
#include <thread>

namespace gfw {

	void runWindow(std::shared_ptr<Window> pSubWindow)
	{
		pSubWindow->run();
	}

	App::App()
		:m_appName("vulkan graphics"),
		m_appVersion(VK_MAKE_VERSION(1, 0, 0)),
		m_engineName("No engine"),
		m_engineVersion(VK_MAKE_VERSION(1, 0, 0)),
		m_width(0u),
		m_height(0u),
		m_title("")
	{
	}

	App::~App()
	{
		m_pWindow = nullptr;
		m_pSubWindows.resize(0);

#ifdef DEBUG
		if (m_debugReportCallBack != vk::DebugReportCallbackEXT(nullptr))
		{
			destroyDebugReportCallbackEXT(*m_pInstance, m_debugReportCallBack, nullptr);
		}
#endif // DEBUG

		glfwTerminate();
	}

	void App::run()
	{
		//std::thread thread_test;

		while (m_pWindow->windowShouldClose() == GFW_FALSE)
		{
			//remove closed child(sub) windows.
			m_pSubWindows.erase(std::remove_if(m_pSubWindows.begin(), m_pSubWindows.end(), [](const std::shared_ptr<Window>& item) {
				return item->windowShouldClose();
			}), m_pSubWindows.end());

			m_pWindow->run();
			ThreadMaster threadMaster;
			for (const auto& pSubWindow : m_pSubWindows)
			{
				threadMaster.appendThread(std::shared_ptr<std::thread>(new std::thread(runWindow, pSubWindow)));
			};

			threadMaster.join();

			glfwPollEvents();
		}

		//vkDeviceWaitIdle(device);
	}

	/*void AppBase::createSubWindow(uint32_t width, uint32_t height, const char *title)
	{
		_createSubWindow(width, height, title);
	}*/

	std::shared_ptr<vk::Instance> App::getVKInstance()
	{
		return m_pInstance;
	}

	std::shared_ptr<vk::PhysicalDevice> App::getPhysicalDevice()
	{
		return m_pPhysicalDevice;
	}

	std::shared_ptr<vk::Device> App::getDevice()
	{
		return m_pDevice;
	}

	vk::Queue App::getGraphicsQueue()
	{
		return m_graphicsQueue;
	}

	vk::Queue App::getPresentQueue()
	{
		return m_presentQueue;
	}

	void App::_initEnv(uint32_t width
		, uint32_t height
		, const char *title
		, std::shared_ptr<GLFWwindow> &pResultGLFWWindow
	    , std::shared_ptr<vk::SurfaceKHR> &pResultSurface)
	{
		m_width = width;
		m_height = height;
		m_title = title;

		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

#ifdef ENABLE_VALIDATION_LAYERS
		if (_checkValidationLayerSupport() == false)
		{
			throw std::runtime_error("Validation layers requested is not available!");
		}
#endif // ENABLE_VALIDATION_LAYERS
		vk::ApplicationInfo appInfo = { m_appName, m_appVersion, m_engineName, m_engineVersion, VK_API_VERSION_1_0 };

		//query application required extensions.
		auto requiredExtensions = _getRequiredExtensions();

#ifdef DEBUG
		//query available extensions.
		auto availableExtensions = vk::enumerateInstanceExtensionProperties();

		//print available extensions.
		LOG(plog::debug) << "Available extensions: " << std::endl;
		for (const auto& extension : availableExtensions)
		{
			LOG(plog::debug) << "\t" << extension.extensionName << std::endl;
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

		pResultGLFWWindow = _createGLFWWindow(m_width, m_height, m_title);
		pResultSurface = _createVKSurface(pResultGLFWWindow);

		_pickPhysicalDevice(pResultSurface);
		_createLogicDevice(pResultSurface);
		_initGrahpicsModule(pResultSurface);
	}

#ifdef ENABLE_VALIDATION_LAYERS
	bool App::_checkValidationLayerSupport()
	{
		//query available layers.
		auto avaibleLayers = vk::enumerateInstanceLayerProperties();

		//print available layers.
		LOG(plog::debug) << "Available layers: " << std::endl;
		for (const auto& layerProperties : avaibleLayers)
		{
			LOG(plog::debug) << "\t" << layerProperties.layerName << std::endl;
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

	std::shared_ptr<GLFWwindow> App::_createGLFWWindow(uint32_t width, uint32_t height, const char* title)
	{
		return fd::createGLFWWindow(width, height, title);
	}

	std::shared_ptr<vk::SurfaceKHR> App::_createVKSurface(std::shared_ptr<GLFWwindow> pWindow)
	{
		return fd::createSurface(m_pInstance, pWindow);
	}

	void App::_pickPhysicalDevice(std::shared_ptr<vk::SurfaceKHR> pSurface)
	{
		auto physicalDevices = m_pInstance->enumeratePhysicalDevices();

		LOG(plog::debug) << "physical device num: " + physicalDevices.size() << std::endl;

		if (physicalDevices.size() == 0)
		{
			throw std::runtime_error("Failed to find GPUs with VULKAN support.");
		}

		const size_t size = physicalDevices.size();

		for (auto it = physicalDevices.cbegin(); it != physicalDevices.cend();)
		{
			Bool32 isSuitable = [&](const vk::PhysicalDevice& physicalDevice)->Bool32
			{
				const vk::PhysicalDeviceProperties deviceProperties = physicalDevice.getProperties();
				const vk::PhysicalDeviceFeatures deviceFeatures = physicalDevice.getFeatures();
				//Application can't function without geometry shaders
				if (deviceFeatures.geometryShader == VK_FALSE)
				{
					return GFW_FALSE;
				}

				//Application can't function without queue family that supports graphics commands.
				if (UsedQueueFamily::findQueueFamilies(physicalDevice, *pSurface).isComplete() == GFW_FALSE)
				{
					return GFW_FALSE;
				}

				//Application can't function without support of device for swap chain extension.
				if (checkDeviceExtensionSupport(physicalDevice, deviceExtensionNames) == GFW_FALSE)
				{
					return GFW_FALSE;
				}

				//Application can't function without adequate support of device for swap chain.
				SwapChainSupportDetails swapChainSupportDetails = SwapChainSupportDetails::querySwapChainSupport(physicalDevice, *pSurface);
				if (swapChainSupportDetails.formats.empty() || swapChainSupportDetails.presentModes.empty())
				{
					return GFW_FALSE;
				}

				//Application can't function without feature of sampler anisotropy.
				if (deviceFeatures.samplerAnisotropy == VK_FALSE)
				{
					return GFW_FALSE;
				}

				return GFW_TRUE;
			}(*it);

			if (isSuitable == false)
			{
				physicalDevices.erase(it);
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

		const std::vector<const char*>& deviceExtensions = deviceExtensionNames;
		std::sort(physicalDevices.begin(), physicalDevices.end(),
			[&](const vk::PhysicalDevice& physicalDevice1, const vk::PhysicalDevice& physicalDevice2) {
			int32_t result = 0;
			const vk::PhysicalDeviceProperties deviceProperties1 = physicalDevice1.getProperties();
			const vk::PhysicalDeviceProperties deviceProperties2 = physicalDevice2.getProperties();
			const vk::PhysicalDeviceFeatures deviceFeatures1 = physicalDevice1.getFeatures();
			const vk::PhysicalDeviceFeatures deviceFeatures2 = physicalDevice2.getFeatures();
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
			return result > 0;
		});

		m_pPhysicalDevice = std::shared_ptr<vk::PhysicalDevice>(new vk::PhysicalDevice(*physicalDevices.cbegin()));
		LOG(plog::debug) << "Pick successfully physical device.";
	}

	void App::_createLogicDevice(std::shared_ptr<vk::SurfaceKHR> pSurface)
	{
		UsedQueueFamily usedQueueFamily = UsedQueueFamily::findQueueFamilies(*m_pPhysicalDevice, *pSurface);
		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
		std::set<int32_t> uniqueFamilyIndices = { usedQueueFamily.graphicsFamily, usedQueueFamily.presentFamily };

		queueCreateInfos.resize(uniqueFamilyIndices.size());
		float queuePriority = 1.0f;

		size_t index = 0;
		for (int32_t queueFamilyIndex : uniqueFamilyIndices)
		{
			vk::DeviceQueueCreateInfo queueCreateInfo = {
				vk::DeviceQueueCreateFlags(),
				static_cast<uint32_t>(queueFamilyIndex),
				uint32_t(1),
				&queuePriority
			};
			queueCreateInfos[index] = queueCreateInfo;
			++index;
		}

		vk::PhysicalDeviceFeatures deviceFeatures = {};
		deviceFeatures.samplerAnisotropy = VK_TRUE;

		vk::DeviceCreateInfo createInfo = {
			vk::DeviceCreateFlags(),
			static_cast<uint32_t>(queueCreateInfos.size()),
			queueCreateInfos.data(),
			0,
			nullptr,
			static_cast<uint32_t>(deviceExtensionNames.size()),
			deviceExtensionNames.data(),
			&deviceFeatures
		};

#ifdef ENABLE_VALIDATION_LAYERS
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationlayers.size());
		createInfo.ppEnabledLayerNames = validationlayers.data();
#endif // ENABLE_VALIDATION_LAYERS

		m_pDevice = fd::createDevice(m_pPhysicalDevice, createInfo);
		m_graphicsQueue = m_pDevice->getQueue(usedQueueFamily.graphicsFamily, 0);
		m_presentQueue = m_pDevice->getQueue(usedQueueFamily.presentFamily, 0);

		LOG(plog::debug) << "Create successfully logic device.";
	}

	void App::_initGrahpicsModule(std::shared_ptr<vk::SurfaceKHR> pSurface)
	{
		UsedQueueFamily usedQueueFamily = UsedQueueFamily::findQueueFamilies(*m_pPhysicalDevice, *pSurface);
		kgs::init(m_pPhysicalDevice, m_pDevice, m_graphicsQueue, usedQueueFamily.graphicsFamily);
	}

	std::vector<const char*> App::_getRequiredExtensions()
	{
		std::vector<const char*> requiredExtensions;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

#ifdef ENABLE_VALIDATION_LAYERS
		uint32_t requiredExtensionCount = glfwExtensionCount + 1;
#else
		uint32_t requiredExtensionCount = glfwExtensionCount;
#endif // ENABLE_VALIDATION_LAYERS

		requiredExtensions.resize(requiredExtensionCount);

		for (uint32_t i = 0; i < glfwExtensionCount; ++i)
		{
			requiredExtensions[i] = glfwExtensions[i];
		}

#ifdef ENABLE_VALIDATION_LAYERS
		requiredExtensions[glfwExtensionCount] = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
#endif // ENABLE_VALIDATION_LAYERS

		return requiredExtensions;
	}

#ifdef DEBUG
	void App::_setupDebugCallBack()
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
		createDebugReportCallbackEXT(*m_pInstance, &VkDebugReportCallbackCreateInfoEXT(createInfo),
			nullptr, &callback);
		m_debugReportCallBack = callback;
	}

	void App::_onDebugCallBack(vk::DebugReportFlagsEXT flags, vk::DebugReportObjectTypeEXT objType,
		uint64_t obj, size_t location, int32_t code, const char* layerPrefix,
		const char* msg)
	{
		if (flags & vk::DebugReportFlagBitsEXT::eDebug)
		{
			LOG(plog::debug) << code << " : " << msg << " at " << layerPrefix << std::endl;
		}
		else if (flags & vk::DebugReportFlagBitsEXT::eInformation)
		{
			LOG(plog::info) << code << " : " << msg << " at " << layerPrefix << std::endl;
		}
		else if (flags & vk::DebugReportFlagBitsEXT::eWarning)
		{
			LOG(plog::warning) << code << " : " << msg << " at " << layerPrefix << std::endl;
		}
		else if (flags & vk::DebugReportFlagBitsEXT::ePerformanceWarning)
		{
			LOG(plog::warning) << code << " : " << msg << " at " << layerPrefix << std::endl;
		}
		else if (flags & vk::DebugReportFlagBitsEXT::eError)
		{
			LOG(plog::error) << code << " : " << msg << " at " << layerPrefix << std::endl;
		}
		else
		{
			LOG(plog::info) << code << " : " << msg << " at " << layerPrefix << std::endl;
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
		App* context = (App *)userData;
		context->_onDebugCallBack(vk::DebugReportFlagsEXT(vk::DebugReportFlagBitsEXT(flags)),
			vk::DebugReportObjectTypeEXT(objType),
			obj, location,
			code,
			layerPrefix,
			msg);

		return VK_FALSE;
	}
#endif // DEBUG

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
}
