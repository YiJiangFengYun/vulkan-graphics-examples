#include "graphics/scene/transform.hpp"

namespace kgs
{	
	template <SpaceType SPACE_TYPE>
	void Transform<SPACE_TYPE>::_setLocalRotationOnly(RotationType rotation)
	{
		m_localRotation = rotation;
		m_isChanged = KGS_TRUE;
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
} //namespace kgs