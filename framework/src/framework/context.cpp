#include <plog/Log.h>
#include "context.hpp"

fw::Context::Context(const char* appName, uint32_t appVersion, const char* engineName, uint32_t engineVersion)
	:m_appName(appName), m_appVersion(appVersion), m_engineName(engineName), m_engineVersion(engineVersion)
{

}

fw::Context::~Context()
{

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