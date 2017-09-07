#include "graphics/global.hpp"

#include <plog/Appenders/DebugOutputAppender.h>

namespace kgs
{
	void initLog()
	{
		//init default log to write to the windows debug output
		static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
		plog::init(plog::verbose, &debugOutputAppender);
	}

	void initGraphics(std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
		std::shared_ptr<vk::Device> pNativeDevice, vk::Queue graphicsQueue,
		std::shared_ptr<vk::CommandPool> pCommandPool)
	{
		initLog();
		pContext = std::shared_ptr<Context>(new Context(pPhysicalDevice, pNativeDevice, graphicsQueue, pCommandPool));
	}
}
