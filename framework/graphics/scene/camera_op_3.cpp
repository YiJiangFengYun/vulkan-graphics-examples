#include "graphics/scene/camera_op_3.hpp"

namespace vg
{
	CameraOP3::CameraOP3()
		: CameraOP<SpaceType::SPACE_3>()
	{
		m_viewBounds = { {-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f} };
	}

	void CameraOP3::apply()
	{
		auto min = m_viewBounds.getMin();
		auto max = m_viewBounds.getMax();
		m_projMatrix[0] = { 2.0f / (max.x - min.x), 0.0f, 0.0f, 0.0f };
		m_projMatrix[1] = { 0.0f, 2.0f / (max.y - min.y), 0.0f, 0.0f };
		m_projMatrix[2] = { 0.0f, 0.0f, 1.0f / (max.z - min.z), 0.0f };
		m_projMatrix[3] = { -(max.x + min.x) / (max.x - min.x)
			, -(max.y + min.y) / (max.y - min.y)
			, -min.z / (max.z - min.z)
		    , 1.0f };
		m_projMatrix[1][1] *= -1;
	}
} //namespace kgs