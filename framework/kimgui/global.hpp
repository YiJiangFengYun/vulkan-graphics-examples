#ifndef VG_KIM_GUI_GLOBAL_H
#define VG_KIM_GUI_GLOBAL_H

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

namespace kimgui {
	typedef vg::Bool32 Bool32;

	//used to map to imgui key map enum.
	enum class Key {
		Tab = ImGuiKey_Tab,       // for tabbing through fields
		LeftArrow = ImGuiKey_LeftArrow, // for text edit
		ImGuiKey_RightArrow,// for text edit
		ImGuiKey_UpArrow,   // for text edit
		ImGuiKey_DownArrow, // for text edit
		ImGuiKey_PageUp,
		ImGuiKey_PageDown,
		ImGuiKey_Home,      // for text edit
		ImGuiKey_End,       // for text edit
		ImGuiKey_Delete,    // for text edit
		ImGuiKey_Backspace, // for text edit
		ImGuiKey_Enter,     // for text edit
		ImGuiKey_Escape,    // for text edit
		ImGuiKey_A,         // for text edit CTRL+A: select all
		ImGuiKey_C,         // for text edit CTRL+C: copy
		ImGuiKey_V,         // for text edit CTRL+V: paste
		ImGuiKey_X,         // for text edit CTRL+X: cut
		ImGuiKey_Y,         // for text edit CTRL+Y: redo
		ImGuiKey_Z,         // for text edit CTRL+Z: undo
		ImGuiKey_COUNT
	};
} //kimgui
#endif //VG_KIM_GUI_GLOBAL_H