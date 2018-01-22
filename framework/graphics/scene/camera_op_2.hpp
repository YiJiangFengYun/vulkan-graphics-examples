#ifndef VG_CAMERA_OP_2_H
#define VG_CAMERA_OP_2_H

#include "graphics/scene/camera_op.hpp"

namespace vg
{
	class CameraOP2 : public CameraOP<SpaceType::SPACE_2>
	{
	public:
		CameraOP2();
		void apply() override;
	private:

	};
} //namespace kgs

#endif // !VG_CAMERA_OP_2_H
