#include "graphics/scene/option.hpp"

namespace kgs
{
	
	template<>
	const MeshType SpaceTypeInfo<SpaceType::SPACE_2>::MeshType = MeshType::SPACE_2;

	template<>
	const MeshType SpaceTypeInfo<SpaceType::SPACE_3>::MeshType = MeshType::SPACE_3;
}