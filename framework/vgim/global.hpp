#ifndef VG_IM_GLOBAL_H
#define VG_IM_GLOBAL_H

#ifdef _DEBUG
#define DEBUG
#endif // _DEBUG

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif // !GLFW_INCLUDE_VULKAN
//#include <GLFW/glfw3.h>

#include <graphics/graphics.hpp>
#include <imgui/imgui.h>

#define ICIMGUI_FALSE VG_FALSE
#define ICIMGUI_TRUE VG_TRUE

namespace vgim {
	typedef vg::Bool32 Bool32;
	// typedef 
} //vgim
#endif //VG_IM_GLOBAL_H