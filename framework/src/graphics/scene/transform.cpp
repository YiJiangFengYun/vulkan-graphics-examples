#include "graphics/scene/transform.hpp"

namespace kgs
{	
	BaseTransform::BaseTransform()
		: Base(BaseType::TRANSFORM)
	{

	}

	template <SpaceType SPACE_TYPE>
	void Transform<SPACE_TYPE>::_setLocalRotationOnly(RotationType rotation)
	{
		throw std::runtime_error("Method cann't be called.");
	}

	template <SpaceType SPACE_TYPE>
	void  Transform<SPACE_TYPE>::setLocalMatrix(MatrixType matrix)
	{
		throw std::runtime_error("Method cann't be called.");
	}

	template <SpaceType SPACE_TYPE>
	void  Transform<SPACE_TYPE>::setLocalMatrixInverse(MatrixType matrix)
	{
		throw std::runtime_error("Method cann't be called.");
	}

	template<>
	void Transform<SpaceType::SPACE_2>::_setLocalRotationOnly(float rotation)
	{
		m_localRotation = rotation;
		Matrix3x3 result(1.0f);
		result[0][0] = glm::cos(rotation);
		result[0][1] = glm::sin(rotation);
		result[1][0] = - glm::sin(rotation);
		result[1][1] = glm::cos(rotation);
		m_localRotationMatrix = result;
		m_isChanged = KGS_TRUE;
	}

	template<>
	void Transform<SpaceType::SPACE_3>::_setLocalRotationOnly(Quaternion rotation)
	{
		m_localRotation = rotation;
		m_localRotationMatrix = glm::toMat4(rotation);
		m_isChanged = KGS_TRUE;
	}

	template <>
	void  Transform<SpaceType::SPACE_3>::setLocalMatrix(MatrixType matrix)
	{
		_setLocalMatrixOnly(matrix);
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
	}


	template <>
	void  Transform<SpaceType::SPACE_3>::setLocalMatrixInverse(MatrixType matrix)
	{
		_setLocalMatrixInverseOnly(matrix);
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
	}
	


	
} //namespace kgs