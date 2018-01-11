#include "graphics/module.hpp"

namespace kgs
{
	std::shared_ptr<Application> pApp;
	Bool32 isInited = KGS_FALSE;
	void moduleCreateVkinstance(std::string name, uint32_t version)
	{
		if (isInited == KGS_TRUE) return;
		fd::moduleCreate();
		pApp = std::shared_ptr<Application>(new Application(name, version));
		pApp->initCreateVkInstance();
	}

	void moduleCreateOther(std::shared_ptr<vk::SurfaceKHR> pSurface
		, uint32_t graphicsQueueCount
		, uint32_t presentQueueCount
		, vk::PhysicalDeviceFeatures needPhysicalDeviceFeatures
	)
	{
		pApp->initOther(pSurface
			, graphicsQueueCount
			, presentQueueCount
			, needPhysicalDeviceFeatures);
		isInited = KGS_TRUE;
	}

	void moduleDestory()
	{
		pApp = nullptr;
		//fd::moduleDestroy();
		isInited = KGS_FALSE;
	}
}
