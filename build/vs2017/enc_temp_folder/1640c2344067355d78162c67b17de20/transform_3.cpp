#include "graphics/scene/transform_3.hpp"

namespace kgs
{
	Transform3::Transform3()
		: Transform<SpaceType::SPACE_3>()
	{

	}

	void Transform3::lookAt(const PointType& worldTarget, const VectorType& worldUp)
	{
		auto matrix = glm::lookAt(getPosition(), worldTarget, worldUp);
		if (m_pParent != nullptr)
		{
			auto m = _getMatrixLocalToWorld(KGS_FALSE);
			matrix = glm::inverse(m) * matrix;
		}

		VectorType tempScale;
		RotationType tempRotation;
		PointType tempTranslate;
		PointType tempSkew;
		MatrixVectorType tempPerspective;
		glm::decompose(matrix, tempScale, tempRotation, tempTranslate, tempSkew, tempPerspective);
		_setLocalScaleOnly(tempScale);
		_setLocalRotationOnly(tempRotation);
		_setLocalPositionOnly(tempTranslate);
		m_isChanged = KGS_FALSE;

		m_localMatrix = matrix;
		//_reCalculateLocalMatrix();
	}

	void Transform3::lookAt2(const PointType& worldEye, const PointType& worldTarget, const VectorType& worldUp)
	{
		auto matrix = glm::lookAt(worldEye, worldTarget, worldUp);
		if (m_pParent != nullptr)
		{
			auto m = _getMatrixLocalToWorld(KGS_FALSE);
			matrix = glm::inverse(m) * matrix;
		}
		setLocalMatrixInverse(matrix);
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
		_setLocalScaleOnly(tempScale);
		_setLocalRotationOnly(tempRotation);
		_setLocalPositionOnly(tempTranslate);
		m_isChanged = KGS_FALSE;

		//_reCalculateLocalMatrix();
	}
} //namespace kgs