#ifndef KGS_LIGHT_H
#define KGS_LIGHT_H

#include "graphics/scene/object.hpp"

namespace kgs
{
	class BaseLight
	{
	public:

	protected:

	};

	template <SpaceType SPACE_TYPE>
	class Light : public BaseLight, public Object<SPACE_TYPE>
	{
		Light()
			: BaseLight()
			, Object<SPACE_TYPE>()
			, m_objectType(ObjectType::LIGHT)
		{

		}
	};
} //namespace kgs

#endif // !KGS_LIGHT_H
