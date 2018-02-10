#ifndef VG_IM_MODULE_H
#define VG_IM_MODULE_H

#include "vgim/global.hpp"

namespace vgim {
	extern Bool32 isInited;
	extern void moduleCreate();
	extern void moduleDestory();
	extern const std::shared_ptr<vg::Material> getMaterial();
	extern const std::shared_ptr<vg::SimpleMesh> getMesh();
	extern const std::shared_ptr<vg::VisualObject2> getUIObject();
	extern const std::shared_ptr<vg::CameraOP2> getCamera();
	extern const std::shared_ptr<vg::Scene2> getScene();
	extern void updateFromImGUI();
} //vgim
#endif //VG_IM_MODULE_H