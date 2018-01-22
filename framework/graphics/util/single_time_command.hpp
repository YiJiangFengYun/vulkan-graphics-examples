#ifndef VG_SINGLE_TIME_COMMAND_H
#define VG_SINGLE_TIME_COMMAND_H

#include <foundation/wrapper.hpp>
#include "graphics/global.hpp"

namespace vg
{
	extern std::shared_ptr<vk::CommandBuffer> beginSingleTimeCommands();

	extern void endSingleTimeCommands(std::shared_ptr<vk::CommandBuffer> pCommandBuffer);
} //namespace kgs
#endif // !VG_SINGLE_TIME_COMMAND_H
