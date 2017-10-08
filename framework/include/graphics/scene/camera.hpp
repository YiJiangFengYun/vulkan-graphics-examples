#ifndef KGS_CAMERA_H
#define KGS_CAMERA_H

#include "graphics/scene/object.hpp"

namespace kgs
{
	template <SpaceType SPACE_TYPE>
	class Camera : Object<SPACE_TYPE>
	{
	public:
		Camera()
			: Object()
			, m_objectType(ObjectType::CAMERA)
		{

		}
	};
} //namespace kgs

#endif // !KGS_CAMERA_H
