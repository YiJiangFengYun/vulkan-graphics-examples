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
		typedef fd::Bounds<typename SpaceTypeInfo<SPACE_TYPE>::PointType> BoundsType;
		Camera()
			: BaseCamera()
			, Object<SPACE_TYPE>()
		{
			m_objectType = ObjectType::CAMERA;
		}

		virtual typename TransformType::MatrixType getProjMatrix() = 0;

		virtual Bool32 isInView(Transform<SPACE_TYPE> *pTransform, BoundsType bounds) = 0;
	};
} //namespace kgs

#endif // !KGS_CAMERA_H
