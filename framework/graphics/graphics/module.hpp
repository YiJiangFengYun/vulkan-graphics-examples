#ifndef VG_MODULE_H
#define VG_MODULE_H

#include "graphics/global.hpp"
#include "graphics/app/app.hpp"
#include "graphics/texture/texture_default.hpp"

namespace vg
{
	extern Bool32 isInited;
	extern void moduleCreate(plog::Severity severity, plog::IAppender *appender = nullptr);
	extern void moduleCreateVkinstance(std::string name
		, uint32_t version
		, std::vector<const char*> vkExtensions);
	extern void moduleCreateOther(std::shared_ptr<vk::SurfaceKHR> pSurface
		, uint32_t graphicsQueueCount
		, uint32_t presentQueueCount
		, vg::PhysicalDeviceFeatures requiredPhysicalDeviceFeatures
		, vg::PhysicalDeviceFeaturePriorities optionalPhysicalDeviceFeatures
	);
	extern void moduleDestory();
	extern void setLogSeverity(plog::Severity severity);
	extern void setVulkanLogSeverity(plog::Severity severity);
	extern void setCostTimeLogSeverity(plog::Severity severity);
} //namespace kgs

#endif // !VG_MODULE_H
