#ifndef KGS_MODULE_H
#define KGS_MODULE_H

#include "graphics/global.hpp"
#include "graphics/app/app.hpp"

namespace kgs
{
	extern Bool32 isInited;
	extern std::shared_ptr<Application> pApp;
	extern void moduleCreateVkinstance(std::string name
		, uint32_t version
		, std::vector<const char*> vkExtensions);
	extern void moduleCreateOther(std::shared_ptr<vk::SurfaceKHR> pSurface
		, uint32_t graphicsQueueCount
		, uint32_t presentQueueCount
		, vk::PhysicalDeviceFeatures needPhysicalDeviceFeatures
	);
	extern void moduleDestory();
} //namespace kgs

#endif // !KGS_MODULE_H
