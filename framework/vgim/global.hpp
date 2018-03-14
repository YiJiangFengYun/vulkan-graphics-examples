#ifndef VG_IM_GLOBAL_H
#define VG_IM_GLOBAL_H

#ifdef _DEBUG
#define DEBUG
#endif // _DEBUG

#include <vgim/config.hpp>

#define VGIM_PLOG_ID _VGIM_PLOG_ID
#define VGIM_LOG(severity) LOG_(VGIM_PLOG_ID, severity)
#define VGIM_IF_LOG(severity) IF_LOG_(VGIM_PLOG_ID, severity)

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif // !GLFW_INCLUDE_VULKAN
//#include <GLFW/glfw3.h>

#include <graphics/graphics.hpp>
#include <imgui/imgui.h>

#define VG_IM_FALSE VG_FALSE
#define VG_IM_TRUE VG_TRUE

namespace vgim {
	using Bool32 = vg::Bool32;
} //vgim
#endif //VG_IM_GLOBAL_H