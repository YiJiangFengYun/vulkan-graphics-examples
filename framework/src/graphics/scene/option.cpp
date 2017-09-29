#include "graphics/scene/option.hpp"

namespace kgs
{
	template<>
	const MeshType SpaceConstInfo<SpaceType::SPACE_2>::MESH_TYPE = MeshType::SPACE_2;

	template<>
	const SpaceTypeInfo<SpaceType::SPACE_2>::VectorType SpaceConstInfo<SpaceType::SPACE_2>::UP_VECTOR = { 0.0f, 1.0f };

	template<>
	const MeshType SpaceConstInfo<SpaceType::SPACE_3>::MESH_TYPE = MeshType::SPACE_3;

	template<>
	const SpaceTypeInfo<SpaceType::SPACE_3>::VectorType SpaceConstInfo<SpaceType::SPACE_3>::UP_VECTOR = { 0.0f, 1.0f, 0.0f };
}