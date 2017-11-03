#ifndef KGS_CAMERA_OP_2_H
#define KGS_CAMERA_OP_2_H

#include "graphics/scene/camera_op.hpp"

namespace kgs
{
	class CameraOP2 : public CameraOP<SpaceType::SPACE_2>
	{
	public:
		CameraOP2();
		void apply() override;
	private:

	};
} //namespace kgs

#endif // !KGS_CAMERA_OP_2_H
