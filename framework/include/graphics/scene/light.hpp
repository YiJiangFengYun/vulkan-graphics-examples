#ifndef KGS_LIGHT_H
#define KGS_LIGHT_H

#include "graphics/scene/object.hpp"

namespace kgs
{
	template <SpaceType SPACE_TYPE>
	class Light : public Object<SPACE_TYPE>
	{
		Light()
			: Object()
			, m_objectType(ObjectType::LIGHT)
		{

		}
	};
} //namespace kgs

#endif // !KGS_LIGHT_H
