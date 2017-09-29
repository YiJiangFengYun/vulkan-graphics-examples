#ifndef KGS_TRANSFORM_3_H
#define KGS_TRANSFORM_3_H

#include "graphics/scene/transform.hpp"

namespace kgs
{
	class Transform3 : public Transform<SpaceType::SPACE_3>
	{
	public:
		void lookAt(const PointType& worldTarget, const VectorType& worldUp = SpaceConstInfo<SpaceType::SPACE_3>::UP_VECTOR);
		void RotateAround(const PointType& point, const VectorType& axis, float angle);
	private:

	};
} //namespace kgs

#endif // !KGS_TRANSFORM_3_H
