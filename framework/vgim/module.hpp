#ifndef VG_IM_MODULE_H
#define VG_IM_MODULE_H

#include "vgim/global.hpp"

namespace vgim {
	extern Bool32 isInited;
	extern void moduleCreate();
	extern void moduleDestory();
	extern void renderImGUI();
} //vgim
#endif //VG_IM_MODULE_H