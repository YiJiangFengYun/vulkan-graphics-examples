#ifndef KGS_CAMERA_OP_3_H
#define KGS_CAMERA_OP_3_H

#include "graphics/scene/camera_op.hpp"

namespace kgs
{
	class CameraOP3 : public CameraOP<SpaceType::SPACE_3>
	{
	public:
		CameraOP3();
		void apply() override;
	private:

	};
} //namespace kgs

#endif // !KGS_CAMERA_OP_3_H
