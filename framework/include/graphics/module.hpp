#ifndef KGS_MODULE_H
#define KGS_MODULE_H

#include "graphics/global.hpp"
#include "graphics/context.hpp"
#include "graphics/app/app.hpp"

namespace kgs
{
	extern Bool32 isInited;
	extern std::shared_ptr<Application> pApp;
	extern std::shared_ptr<Context> pContext;
	extern void moduleCreateVkinstance(std::string name, uint32_t version);
	extern void moduleCreateOther(std::shared_ptr<vk::SurfaceKHR> pSurface);
	extern void moduleDestory();
} //namespace kgs

#endif // !KGS_MODULE_H
