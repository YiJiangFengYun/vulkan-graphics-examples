#ifndef KGS_OBJECT_H
#define KGS_OBJECT_H

#include "graphics/global.hpp"
#include "graphics/scene/space_info.hpp"
#include "graphics/scene/space_transform_type_info.hpp"

namespace kgs
{
	template <SpaceType SPACE_TYPE>
	class Object
	{
	public:
		typedef typename SpaceTransformTypeInfo<SPACE_TYPE>::TransformType TransformType;

	protected:
		TransformType m_transform;
	};

} //namespace kgs

#endif // !KGS_OBJECT_H
