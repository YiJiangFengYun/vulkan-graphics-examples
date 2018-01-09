#ifndef KGS_TRANSFORM_3_H
#define KGS_TRANSFORM_3_H

#include "graphics/scene/transform.hpp"

namespace kgs
{
	class Transform3 : public Transform<SpaceType::SPACE_3>
	{
	public:
		Transform3();
		void lookAt(const PointType& worldTarget, const VectorType& worldUp = SpaceConstInfo<SpaceType::SPACE_3>::UP_VECTOR);
		void lookAt2(const PointType& worldEye, const PointType& worldTarget, const VectorType& worldUp = SpaceConstInfo<SpaceType::SPACE_3>::UP_VECTOR);
		void rotateAround(const PointType& point, const VectorType& axis, const float& angle, const VectorType& scale);
	private:

	};
} //namespace kgs

#endif // !KGS_TRANSFORM_3_H
