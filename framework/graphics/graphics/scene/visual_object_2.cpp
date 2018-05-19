#include "graphics/scene/visual_object_2.hpp"

#include "graphics/util/gemo_util.hpp"

namespace vg
{
	VisualObject2::VisualObject2()
		: VisualObject<SpaceType::SPACE_2>()
	{

	}

    Matrix4x4 VisualObject2::_getModelMatrix() const
	{
		return tranMat3ToMat4(m_pTransform->getMatrixLocalToWorld());
	}

} //namespace kgs