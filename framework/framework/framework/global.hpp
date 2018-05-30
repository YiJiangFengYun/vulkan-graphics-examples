#ifndef VGF_GLOBAL_H
#define VGF_GLOBAL_H

#define NOMINMAX
#define VGF_FALSE 0
#define VGF_TRUE 1

#include <cstdint>

#include <foundation/foundation.hpp>
#include <graphics/graphics.hpp>
#include <graphics_ext/graphics_ext.hpp>

#include <framework/config.hpp>

#define VGF_PLOG_ID _VGF_PLOG_ID
#define VGF_LOG(severity) LOG_(VGF_PLOG_ID, severity)
#define VGF_IF_LOG(severity) IF_LOG_(VGF_PLOG_ID, severity)

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
}

#endif // !VGF_GLOBAL_H



