#ifndef KGS_RENDERER_OPTION_H
#define KGS_RENDERER_OPTION_H


#include "graphics/material/material_data.hpp"

namespace kgs
{
	struct RenderInfo {
		uint32_t                         waitSemaphoreCount;
		const vk::Semaphore*             pWaitSemaphores;
		uint32_t                         signalSemaphoreCount;
		const vk::Semaphore*             pSignalSemaphores;
	};

} //namespace kgs

#endif // !KGS_RENDERER_OPTION_H
