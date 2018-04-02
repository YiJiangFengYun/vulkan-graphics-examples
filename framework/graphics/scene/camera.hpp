#ifndef VG_CAMERA_H
#define VG_CAMERA_H

#include "graphics/scene/object.hpp"

namespace vg
{
	// enum class Camera
	class BaseCamera
	{
	public:
	    BaseCamera();
		virtual ~BaseCamera();
		Bool32 getIsOrthographic() const;		
	protected:
	    Bool32 m_isOrthographic;
	};
	template <SpaceType SPACE_TYPE>
	class Camera : public BaseCamera, public Object<SPACE_TYPE>
	{
	public:
		using BoundsType = fd::Bounds<typename SpaceTypeInfo<SPACE_TYPE>::PointType>;
		Camera()
			: BaseCamera()
			, Object<SPACE_TYPE>()
		{
			m_objectType = ObjectType::CAMERA;
		}

		virtual typename TransformType::MatrixType getProjMatrix() const = 0;
	};
} //namespace kgs

#endif // !VG_CAMERA_H
