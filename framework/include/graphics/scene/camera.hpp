#ifndef KGS_CAMERA_H
#define KGS_CAMERA_H

#include "graphics/scene/object.hpp"

namespace kgs
{
	class BaseCamera
	{
	public:

	protected:

	};
	template <SpaceType SPACE_TYPE>
	class Camera : public BaseCamera, public Object<SPACE_TYPE>
	{
	public:
		Camera()
			: BaseCamera()
			, Object<SPACE_TYPE>()
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
