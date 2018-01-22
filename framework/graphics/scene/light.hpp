#ifndef VG_LIGHT_H
#define VG_LIGHT_H

#include "graphics/scene/object.hpp"

namespace vg
{
	class BaseLight
	{
	public:

	protected:

	};

	template <SpaceType SPACE_TYPE>
	class Light : public BaseLight, public Object<SPACE_TYPE>
	{
	public:
		Light()
			: BaseLight()
			, Object<SPACE_TYPE>()
		{
			m_objectType = ObjectType::LIGHT;
		}
	};
} //namespace kgs

#endif // !VG_LIGHT_H
