#include "graphics/scene/camera_op_2.hpp"

namespace kgs
{
	CameraOP2::CameraOP2()
		: CameraOP<SpaceType::SPACE_2>()
	{

	}

	void CameraOP2::apply()
	{
		auto min = m_viewBounds.getMin();
		auto max = m_viewBounds.getMax();
		m_projMatrix[0] = { 2.0f / (max.x - min.x), 0.0f, 0.0f };
		m_projMatrix[1] = {0.0f, 2.0f / (max.y - min.y), 0.0f };
		m_projMatrix[2] = { -(max.x + min.x) / (max.x - min.x), -(max.y + min.y) / (max.y - min.y), 1.0f };
		m_projMatrix[1][1] *= -1;
	}
} //namespace kgs