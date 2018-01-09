#ifndef GFW_GLOBAL_H
#define GFW_GLOBAL_H

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif // !GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GFW_FALSE 0
#define GFW_TRUE 1

#include <cstdint>
#include <foundation/foundation.hpp>
#include <graphics/graphics.hpp>

namespace gfw
{ 
	typedef uint32_t Bool32;
	extern Bool32 isInited;
	extern void moduleCreate();
	extern void moduleDestroy();
}

#endif // !GFW_GLOBAL_H



