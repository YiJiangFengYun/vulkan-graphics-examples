#ifndef KGS_OBJECT_H
#define KGS_OBJECT_H

#include "graphics/global.hpp"
#include "graphics/scene/space_info.hpp"
#include "graphics/scene/space_info_object.hpp"

namespace kgs
{
	enum class ObjectType
	{
		UNDEFINED,
		VISUAL_OBJECT,
		CAMERA,
		LIGHT,
		BEGIN_RANGE = VISUAL_OBJECT,
		END_RANGE = LIGHT,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};

	template <SpaceType SPACE_TYPE>
	class Object
	{
	public:
		typedef typename SpaceTransformTypeInfo<SPACE_TYPE>::TransformType TransformType;

		Object()
		{

		}

		ObjectType getObjectType()
		{
			return m_objectType;
		}

	protected:
		ObjectType m_objectType;
		TransformType m_transform;
	};

} //namespace kgs

#endif // !KGS_OBJECT_H
