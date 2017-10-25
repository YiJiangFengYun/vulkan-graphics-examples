#include "graphics/scene/transform.hpp"

namespace kgs
{	
	template <SpaceType SPACE_TYPE>
	void Transform<SPACE_TYPE>::setLocalRotation(RotationType rotation)
	{
		m_localRotation = rotation;
		m_isChanged = KGS_TRUE;
		_reCalculateLocalMatrix();
	}

	template<>
	void Transform<SpaceType::SPACE_2>::setLocalRotation(float rotation)
	{
		m_localRotation = rotation;

		Matrix3x3 result(1.0f);
		result[0][0] = glm::cos(rotation);
		result[0][1] = glm::sin(rotation);
		result[1][0] = - glm::sin(rotation);
		result[1][1] = glm::cos(rotation);
		m_localRotationMatrix = result;

		_reCalculateLocalMatrix();
	}

	template<>
	void Transform<SpaceType::SPACE_3>::setLocalRotation(Quaternion rotation)
	{
		m_localRotation = rotation;
		m_localRotationMatrix = glm::toMat4(rotation);
		_reCalculateLocalMatrix();
	}
} //namespace kgs