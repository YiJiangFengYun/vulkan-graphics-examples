#include "graphics/scene/visual_object_3.hpp"

namespace vg
{
	VisualObject3::VisualObject3()
		: VisualObject<SpaceType::SPACE_3>()
	{

	}

	Matrix4x4 VisualObject3::_getModelMatrix() const
	{
		return m_pTransform->getMatrixLocalToWorld();
	}
} //namespace kgs