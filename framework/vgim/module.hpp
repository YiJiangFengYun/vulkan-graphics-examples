#ifndef VG_IM_MODULE_H
#define VG_IM_MODULE_H

#include "vgim/global.hpp"

namespace vgim {
	extern Bool32 isInited;
	extern void moduleCreate();
	extern void moduleDestory();
	/*extern std::shared_ptr<GLFWwindow> getCurrWindow();
	extern void setCurrWindow(std::shared_ptr<GLFWwindow> pWindow);*/
} //vgim
#endif //VG_IM_MODULE_H