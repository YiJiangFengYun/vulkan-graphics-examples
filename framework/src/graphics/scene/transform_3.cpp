#include "graphics/scene/transform_3.hpp"

namespace kgs
{
	Transform3::Transform3()
		: Transform<SpaceType::SPACE_3>()
	{

	}

	void Transform3::lookAt(const PointType& worldTarget, const VectorType& worldUp)
	{
		MatrixType matrix = _getMatrixWorldToLocal(KGS_TRUE);
		//transform target to local
		PointType localTarget = matrix * MatrixVectorType(worldTarget, 1.0f);
		VectorType localUp = matrix * MatrixVectorType(worldUp, 0.0f);
		m_localMatrix *= glm::lookAt(m_localPosition, localTarget, localUp);

		VectorType tempScale;
		RotationType tempRotation;
		PointType tempTranslate;
		PointType tempSkew;
		MatrixVectorType tempPerspective;
		glm::decompose(m_localMatrix, tempScale, tempRotation, tempTranslate, tempSkew, tempPerspective);
		setLocalScale(tempScale);
		setLocalRotation(tempRotation);
		setLocalPosition(tempTranslate);
		m_isChanged = KGS_FALSE;
	}

	void Transform3::rotateAround(const PointType& point, const VectorType& axis, const float& angle, const VectorType& scale)
	{
		m_localMatrix = glm::translate(m_localMatrix, point);
		m_localMatrix = glm::rotate(m_localMatrix, angle, axis);
		m_localMatrix = glm::scale(m_localMatrix, scale);
		m_localMatrix = glm::translate(m_localMatrix, -point);

		VectorType tempScale;
		RotationType tempRotation;
		PointType tempTranslate;
		PointType tempSkew;
		MatrixVectorType tempPerspective;
		glm::decompose(m_localMatrix, tempScale, tempRotation, tempTranslate, tempSkew, tempPerspective);
		setLocalScale(tempScale);
		setLocalRotation(tempRotation);
		setLocalPosition(tempTranslate);
		m_isChanged = KGS_FALSE;

	}
} //namespace kgs