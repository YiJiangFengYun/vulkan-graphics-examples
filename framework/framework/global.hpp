#ifndef VGF_GLOBAL_H
#define VGF_GLOBAL_H

#define NOMINMAX
#define VGF_FALSE 0
#define VGF_TRUE 1

#include <cstdint>

#include <foundation/foundation.hpp>
#include <graphics/graphics.hpp>
#include <vgim/vgim.hpp>

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif // !GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vgf
{ 
	typedef uint32_t Bool32;
	extern Bool32 isInited;
	extern void moduleCreate();
	extern void moduleDestroy();
}

#endif // !VGF_GLOBAL_H



