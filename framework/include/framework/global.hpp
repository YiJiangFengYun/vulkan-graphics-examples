#ifndef GFW_GLOBAL_H
#define GFW_GLOBAL_H


#ifdef _DEBUG
#define DEBUG
#endif // _DEBUG

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif // !GLFW_INCLUDE_VULKAN

#define GFW_FALSE 0
#define GFW_TRUE 1

#include <cstdint>

namespace gfw
{ 
	typedef uint32_t Bool32;
	extern Bool32 isInited;
	extern void init();
}

#endif // !GFW_GLOBAL_H



