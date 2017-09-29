#include "graphics/scene/transform_3.hpp"

namespace kgs
{
	void Transform3::lookAt(const PointType& worldTarget, const VectorType& worldUp)
	{
		MatrixType matrix = _getMatrixWorldToLocal(KGS_TRUE);
		//transform target to local
		PointType localTarget = matrix * MatrixVectorType(worldTarget, 1.0f);
		VectorType localUp = matrix * MatrixVectorType(worldUp, 0.0f);
		m_localMatrix *= glm::lookAt(m_appliedLocalPosition, localTarget, localUp);
		PointType skew;
		MatrixVectorType perspective;
		glm::decompose(m_localMatrix, m_appliedLocalScale, m_appliedLocalRotation, m_appliedLocalPosition, skew, perspective);
		m_localScale = m_appliedLocalScale;
		m_localRotation = m_appliedLocalRotation;
		m_localPosition = m_appliedLocalPosition;
		m_isChanged = KGS_FALSE;
	}

	void Transform3::RotateAround(const PointType& point, const VectorType& axis, float angle)
	{

	}
} //namespace kgs