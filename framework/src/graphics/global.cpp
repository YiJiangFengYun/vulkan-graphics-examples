#include "graphics/global.hpp"

#include "foundation/global.hpp"
namespace kgs
{
	std::shared_ptr<Context> pContext;
	Bool32 isInited = KGS_FALSE;
	void init(std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
		std::shared_ptr<vk::Device> pNativeDevice, vk::Queue graphicsQueue,
		std::shared_ptr<vk::CommandPool> pCommandPool)
	{
		if (isInited == KGS_TRUE) return;
		fd::init();
		pContext = std::shared_ptr<Context>(new Context(pPhysicalDevice, pNativeDevice, graphicsQueue, pCommandPool));
		isInited = KGS_TRUE;
	}
}
