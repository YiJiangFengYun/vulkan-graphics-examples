#include "graphics/module.hpp"

namespace vg
{
	std::shared_ptr<Application> pApp;
	Bool32 isInited = VG_FALSE;
	void moduleCreateVkinstance(std::string name, uint32_t version, std::vector<const char*> vkExtensions)
	{
		if (isInited == VG_TRUE) return;
		fd::moduleCreate();
		pApp = std::shared_ptr<Application>(new Application(name, version));
		pApp->initCreateVkInstance(vkExtensions);
	}

	void moduleCreateOther(std::shared_ptr<vk::SurfaceKHR> pSurface
		, uint32_t graphicsQueueCount
		, uint32_t presentQueueCount
		, vg::PhysicalDeviceFeatures requiredPhysicalDeviceFeatures
		, vg::PhysicalDeviceFeaturePriorities optionalPhysicalDeviceFeatures
	)
	{
		pApp->initOther(pSurface
			, graphicsQueueCount
			, presentQueueCount
			, requiredPhysicalDeviceFeatures
			, optionalPhysicalDeviceFeatures
			);
		isInited = VG_TRUE;
	}

	void moduleDestory()
	{
		pApp = nullptr;
		//fd::moduleDestroy();
		isInited = VG_FALSE;
	}
}
