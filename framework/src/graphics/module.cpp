#include "graphics/module.hpp"

namespace kgs
{
	std::shared_ptr<Application> pApp;
	std::shared_ptr<Context> pContext;
	Bool32 isInited = KGS_FALSE;
	void moduleCreateVkinstance(std::string name, uint32_t version)
	{
		if (isInited == KGS_TRUE) return;
		fd::moduleCreate();
		pApp = std::shared_ptr<Application>(new Application(name, version));
		pApp->initCreateVkInstance();
	}

	void moduleCreateOther(std::shared_ptr<vk::SurfaceKHR> pSurface)
	{
		pApp->initOther(pSurface);
		auto pPhysicalDevice = pApp->getPhysicalDevice();
		auto pNativeDevice = pApp->getDevice();
		auto graphicsQueue = pApp->getGraphicsQueue();
		UsedQueueFamily usedQueueFamily = UsedQueueFamily::findQueueFamilies(*pPhysicalDevice, *pSurface);
		auto graphicsFamily = usedQueueFamily.graphicsFamily;
		pContext = std::shared_ptr<Context>(new Context(pPhysicalDevice, pNativeDevice, graphicsQueue, graphicsFamily));
		isInited = KGS_TRUE;
	}

	void moduleDestory()
	{
		pContext = nullptr;
		pApp = nullptr;
		//fd::moduleDestroy();
		isInited = KGS_FALSE;
	}
}
