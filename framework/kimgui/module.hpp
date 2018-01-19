#ifndef KGS_KIM_GUI_MODULE_H
#define KGS_KIM_GUI_MODULE_H

#include "kimgui/global.hpp"

namespace kimgui {
	extern Bool32 isInited;
	extern void moduleCreate();
	extern void moduleDestory();
	extern std::shared_ptr<GLFWwindow> getCurrWindow();
	extern void setCurrWindow(std::shared_ptr<GLFWwindow> pWindow);
} //kimgui
#endif //KGS_KIM_GUI_MODULE_H