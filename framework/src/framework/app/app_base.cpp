#include "framework/app/app_base.hpp"

#include <plog/Log.h>
#include <plog/Appenders/DebugOutputAppender.h>
#include <set>
#include <thread>

namespace gfw {

	void runWindow(std::shared_ptr<Window> pSubWindow)
	{
		pSubWindow->run();
	}

	AppBase::AppBase(uint32_t width, uint32_t height, const char *title)
		:m_appName("vulkan graphics"),
		m_appVersion(VK_MAKE_VERSION(1, 0, 0)),
		m_engineName("No engine"),
		m_engineVersion(VK_MAKE_VERSION(1, 0, 0)),
		m_width(width),
		m_height(height),
		m_title(title)
	{
		_init();
	}

	AppBase::~AppBase()
	{
		m_pWindow = nullptr;
		m_pSubWindows.resize(0);

		if (m_device != vk::Device(nullptr))
		{
			m_device.destroy();

		}

#ifdef DEBUG
		if (m_debugReportCallBack != vk::DebugReportCallbackEXT(nullptr))
		{
			destroyDebugReportCallbackEXT(m_instance, m_debugReportCallBack, nullptr);
		}
#endif // DEBUG

		if (m_instance != vk::Instance(nullptr))
		{
			m_instance.destroy();
		}

		glfwTerminate();
	}

	void AppBase::run()
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

	vk::Instance AppBase::getVKInstance()
	{
		return m_instance;
	}

	vk::PhysicalDevice AppBase::getPhysicalDevice()
	{
		return m_physicalDevice;
	}

	vk::Device AppBase::getDevice()
	{
		return m_device;
	}

	vk::Queue AppBase::getGraphicsQueue()
	{
		return m_graphicsQueue;
	}

	vk::Queue AppBase::getPresentQueue()
	{
		return m_presentQueue;
	}

	void AppBase::_init()
	{
		//init default log to write to the windows debug output
		static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
		plog::init(plog::verbose, &debugOutputAppender);

		LOG(plog::debug) << "Application initialization.";

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
		m_instance = vk::createInstance(createInfo);

#ifdef DEBUG
		_setupDebugCallBack();
#endif // DEBUG

		GLFWwindow* pWindow = _createGLFWWindow(m_width, m_height, m_title);
		vk::SurfaceKHR surface = _createVKSurface(pWindow);

		_pickPhysicalDevice(surface);
		_createLogicDevice(surface);
		_createWindow(pWindow, surface);

		LOG(plog::debug) << "Application initialization complete.";
	}

	bool AppBase::_checkValidationLayerSupport()
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

	GLFWwindow* AppBase::_createGLFWWindow(uint32_t width, uint32_t height, const char* title)
	{
		return glfwCreateWindow(width, height, title, nullptr, nullptr);
	}

	vk::SurfaceKHR AppBase::_createVKSurface(GLFWwindow* pWindow)
	{
		VkSurfaceKHR surface;
		auto result = static_cast<vk::Result > (glfwCreateWindowSurface(m_instance, pWindow, nullptr, &surface));
		if (result != vk::Result::eSuccess)
		{
			throw std::system_error(result, "AppBase::_createSurface");
		}
		return surface;
	}

	void AppBase::_pickPhysicalDevice(vk::SurfaceKHR surface)
	{
		auto physicalDevices = m_instance.enumeratePhysicalDevices();

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
				if (UsedQueueFamily::findQueueFamilies(physicalDevice, surface).isComplete() == GFW_FALSE)
				{
					return GFW_FALSE;
				}

				//Application can't function without support of device for swap chain extension.
				if (checkDeviceExtensionSupport(physicalDevice, deviceExtensionNames) == GFW_FALSE)
				{
					return GFW_FALSE;
				}

				//Application can't function without adequate support of device for swap chain.
				SwapChainSupportDetails swapChainSupportDetails = SwapChainSupportDetails::querySwapChainSupport(physicalDevice, surface);
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

		m_physicalDevice = *physicalDevices.cbegin();
		//m_physicalDevice = VkPhysicalDevice(*physicalDevices.b)
		LOG(plog::debug) << "Pick successfully physical device.";
	}

	void AppBase::_createLogicDevice(vk::SurfaceKHR surface)
	{
		UsedQueueFamily usedQueueFamily = UsedQueueFamily::findQueueFamilies(m_physicalDevice, surface);
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

		m_device = m_physicalDevice.createDevice(createInfo);
		m_graphicsQueue = m_device.getQueue(usedQueueFamily.graphicsFamily, 0);
		m_presentQueue = m_device.getQueue(usedQueueFamily.presentFamily, 0);

		LOG(plog::debug) << "Create successfully logic device.";
	}

	void AppBase::_createWindow(GLFWwindow *pwindow, vk::SurfaceKHR surface)
	{
		m_pWindow.reset(new Window(pwindow, surface, m_instance, m_physicalDevice, m_device,
			m_graphicsQueue, m_presentQueue));
	}

	/*std::shared_ptr<Window> AppBase::_createSubWindow(uint32_t width, uint32_t height, const char* title)
	{
		std::shared_ptr<Window> window(new Window(width, height, title, m_instance, m_physicalDevice,
			m_device, m_graphicsQueue, m_presentQueue));
		m_pSubWindows.push_back(window);
		return window;
	}*/


	std::vector<const char*> AppBase::_getRequiredExtensions()
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
	void AppBase::_setupDebugCallBack()
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
		createDebugReportCallbackEXT(m_instance, &VkDebugReportCallbackCreateInfoEXT(createInfo),
			nullptr, &callback);
		m_debugReportCallBack = callback;
	}

	void AppBase::_onDebugCallBack(vk::DebugReportFlagsEXT flags, vk::DebugReportObjectTypeEXT objType,
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
		AppBase* context = (AppBase *)userData;
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
