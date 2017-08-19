#include <plog/Log.h>
#include <GLFW/glfw3.h>

#include "context.hpp"

fw::Context::Context(const char* appName, uint32_t appVersion, const char* engineName, uint32_t engineVersion)
	:m_appName(appName), m_appVersion(appVersion),
	m_engineName(engineName), m_engineVersion(engineVersion),
	m_instance(nullptr),
	m_debugReportCallBack(nullptr)
{

}

fw::Context::~Context()
{
#ifdef DEBUG
	if (m_debugReportCallBack != vk::DebugReportCallbackEXT(nullptr))
	{
		fw::destroyDebugReportCallbackEXT(m_instance, m_debugReportCallBack, nullptr);
	}
#endif // DEBUG

	if (m_instance != vk::Instance(nullptr))
	{
		m_instance.destroy();
	}
}

void fw::Context::init()
{
#ifdef ENABLE_VALIDATION_LAYERS
	if (_checkValidationLayerSupport() == false)
	{
		throw std::runtime_error("validation layers requested is not available!");
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


}

bool fw::Context::_checkValidationLayerSupport()
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

std::vector<const char*> fw::Context::_getRequiredExtensions()
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

	for (int32_t i = 0; i < glfwExtensionCount; ++i)
	{
		requiredExtensions[i] = glfwExtensions[i];
	}

#ifdef ENABLE_VALIDATION_LAYERS
	requiredExtensions[glfwExtensionCount] = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
#endif // ENABLE_VALIDATION_LAYERS

	return requiredExtensions;
}

#ifdef DEBUG

void fw::Context::_setupDebugCallBack()
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
	fw::createDebugReportCallbackEXT(m_instance, &VkDebugReportCallbackCreateInfoEXT(createInfo), 
		nullptr, &callback);
	m_debugReportCallBack = callback;
}

void fw::Context::_onDebugCallBack(vk::DebugReportFlagsEXT flags, vk::DebugReportObjectTypeEXT objType,
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
VKAPI_ATTR VkBool32 VKAPI_CALL fw::debugCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t obj,
	size_t location,
	int32_t code,
	const char* layerPrefix,
	const char* msg,
	void * userData)
{
	Context* context = (Context *)userData;
	context->_onDebugCallBack(vk::DebugReportFlagsEXT(vk::DebugReportFlagBitsEXT(flags)),
		vk::DebugReportObjectTypeEXT(objType),
		obj, location,
		code,
		layerPrefix,
		msg);

	return VK_FALSE;
}
#endif // DEBUG

