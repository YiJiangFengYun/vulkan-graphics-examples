#ifndef VG_SPACE_TRANSFORM_TYPE_INFO_H
#define VG_SPACE_TRANSFORM_TYPE_INFO_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/transform_2.hpp"
#include "graphics/scene/transform_3.hpp"

namespace vg
{
	template<SpaceType type>
	struct SpaceTransformTypeInfo
	{
		using TransformType = void;
	};

	template<>
	struct SpaceTransformTypeInfo<SpaceType::SPACE_2>
	{
		using TransformType = Transform2;
	};

	template<>
	struct SpaceTransformTypeInfo<SpaceType::SPACE_3>
	{
		using TransformType = Transform3;
	};
} //namespace kgs

#endif // !VG_SPACE_TRANSFORM_TYPE_INFO_H
