#ifndef KGS_SCENE_OPTION_H
#define KGS_SCENE_OPTION_H

#include "graphics/mesh/mesh_option.hpp"

namespace kgs
{
	enum class SpaceType
	{
		SPACE_2,
		SPACE_3
	};

	template<SpaceType type>
	struct SpaceTypeInfo
	{
		typedef void VecType;
		const static MeshType MeshType;
	};

	template<>
	struct SpaceTypeInfo<SpaceType::SPACE_2>
	{
		typedef Vector2 VecType;
		const static MeshType MeshType = MeshType::SPACE_2;
	};

	template<>
	struct SpaceTypeInfo<SpaceType::SPACE_3>
	{
		typedef Vector3 VecType;
		const static MeshType MeshType = MeshType::SPACE_3;
	};
} //namespace kgs

#endif // !KGS_SCENE_OPTION_H
