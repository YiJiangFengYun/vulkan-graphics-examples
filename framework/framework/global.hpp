#ifndef VGF_GLOBAL_H
#define VGF_GLOBAL_H

#define NOMINMAX
#define VGF_FALSE 0
#define VGF_TRUE 1

#include <cstdint>

#include <foundation/foundation.hpp>
#include <graphics/graphics.hpp>

#include <framework/config.hpp>

#ifdef USE_IMGUI_BIND
#include <vgim/vgim.hpp>
#endif //USE_IMGUI_BIND

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif // !GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vgf
{ 
	using Bool32 = uint32_t;
	extern Bool32 isInited;
	extern void moduleCreate();
	extern void moduleDestroy();
}

#endif // !VGF_GLOBAL_H



