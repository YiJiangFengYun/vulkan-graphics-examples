#ifndef VG_IM_MODULE_H
#define VG_IM_MODULE_H

#include "vgim/global.hpp"

namespace vgim {
	enum class DimType
	{
		SPACE_2 = static_cast<int32_t>(vg::MeshDimType::SPACE_2),
		SPACE_3 = static_cast<int32_t>(vg::MeshDimType::SPACE_3),
		BEGIN_RANGE = SPACE_2,
		END_RANGE = SPACE_3,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};

	

	extern Bool32 isInited;
	extern void moduleCreate(DimType dimensionType);
	extern void moduleDestory();
	extern const std::shared_ptr<vg::Material> getMaterial();
	extern const std::shared_ptr<vg::SimpleMesh> getMesh();
	extern const std::shared_ptr<vg::VisualObject2> getUIObject();
	extern const std::shared_ptr<vg::CameraOP2> getCamera();
	extern const std::shared_ptr<vg::Scene2> getScene();
	extern void updateFromImGUI();
} //vgim
#endif //VG_IM_MODULE_H