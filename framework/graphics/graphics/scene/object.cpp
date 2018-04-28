#include "graphics/scene/object.hpp"

namespace vg
{
	BaseObject::BaseObject()
	{

	}

	ObjectType BaseObject::getObjectType()
	{
		return m_objectType;
	}
} //namespace kgs