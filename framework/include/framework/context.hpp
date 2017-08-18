#ifndef FW_CONTEXT_H
#define FW_CONTEXT_H

#include <vector>
#include <vulkan/vulkan.hpp>

namespace fw
{
#ifdef _DEBUG
#define ENABLE_VALIDATION_LAYERS
	const std::vector<const char*> validationlayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};
#endif // DEBUG

	class Context
	{
	public:
		Context(const char* appName, uint32_t appVersion, const char* engineName, uint32_t engineVersion);
		~Context();
		void init();
	private:
		const char* m_appName;
		uint32_t m_appVersion;
		const char* m_engineName;
		uint32_t m_engineVersion;
		bool _checkValidationLayerSupport();
	};
}

#endif // !FW_CONTEXT_H

