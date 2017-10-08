#ifndef KGS_CAMERA_H
#define KGS_CAMERA_H

#include "graphics/scene/object.hpp"

namespace kgs
{
	template <SpaceType SPACE_TYPE>
	class Camera : public Object<SPACE_TYPE>
	{
	public:
		Camera()
			: Object()
		{
			m_objectType = ObjectType::CAMERA;
		}

		virtual typename TransformType::MatrixType getProjMatrix()
		{
			return TransformType::MatrixType(1.0f);
		}
	};
} //namespace kgs

#endif // !KGS_CAMERA_H
