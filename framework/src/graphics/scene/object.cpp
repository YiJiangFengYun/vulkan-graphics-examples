#include "graphics/scene/object.hpp"

namespace kgs
{
	BaseObject::BaseObject()
	{

	}

	ObjectType BaseObject::getObjectType()
	{
		return m_objectType;
	}
} //namespace kgs