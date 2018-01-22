#include "graphics/scene/object.hpp"

namespace vg
{
	BaseObject::BaseObject()
		: Base(BaseType::SCENE_OBJECT)
	{

	}

	ObjectType BaseObject::getObjectType()
	{
		return m_objectType;
	}
} //namespace kgs