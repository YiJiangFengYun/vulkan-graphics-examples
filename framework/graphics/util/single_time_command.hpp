#ifndef KGS_SINGLE_TIME_COMMAND_H
#define KGS_SINGLE_TIME_COMMAND_H

#include <foundation/wrapper.hpp>
#include "graphics/global.hpp"

namespace kgs
{
	extern std::shared_ptr<vk::CommandBuffer> beginSingleTimeCommands();

	extern void endSingleTimeCommands(std::shared_ptr<vk::CommandBuffer> pCommandBuffer);
} //namespace kgs
#endif // !KGS_SINGLE_TIME_COMMAND_H
