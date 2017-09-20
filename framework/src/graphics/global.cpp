#include "graphics/global.hpp"

#include "foundation/global.hpp"
namespace kgs
{
	const Color COLOR_ZERO = { 0, 0, 0, 0 };
	const Color COLOR_WHITE = { 1, 1, 1, 1 };
	const Color COLOR_BLACK = { 0, 0, 0, 1 };
	const Color COLOR_BLUE = { 0, 0, 1, 1 };
	const Color COLOR_GREEN = { 0, 1, 0, 1 };
	const Color COLOR_RED = { 1, 0, 0, 1 };
	const Color COLOR_YELLOW = { 1, 1, 0, 1 };
	const Color COLOR_MAGENTA = { 1, 0, 1, 1 };
	const Color COLOR_CYAN = { 0, 1, 1, 1 };
	const Color COLOR_GRAY = { 0.5, 0.5, 0.5, 1 };

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
