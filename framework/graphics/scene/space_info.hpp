#ifndef VG_SCENE_OPTION_H
#define VG_SCENE_OPTION_H

#include "graphics/global.hpp"
#include "graphics/mesh/mesh_option.hpp"

namespace vg
{
	enum class SpaceType
	{
		SPACE_2,
		SPACE_3,
		BEGIN_RANGE = SPACE_2,
		END_RANGE = SPACE_3,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};

	template<SpaceType type>
	struct SpaceTypeInfo
	{
		static const MeshDimType MESH_DIM_TYPE;
		typedef void VectorType;
		typedef void PointType;
		typedef void MatrixVectorType;
		typedef void MatrixType;
		typedef void RotationType;
	};

	template<SpaceType type>
	struct SpaceConstInfo
	{
		static const typename SpaceTypeInfo<type>::VectorType UP_VECTOR;
	};

	template<>
	struct SpaceTypeInfo<SpaceType::SPACE_2>
	{
		static const MeshDimType MESH_DIM_TYPE = MeshDimType::SPACE_2;
		typedef Vector2 VectorType;
		typedef Vector2 PointType;
		typedef Vector3 MatrixVectorType;
		typedef Matrix3x3 MatrixType;
		typedef float RotationType;
	};

	template<>
	struct SpaceTypeInfo<SpaceType::SPACE_3>
	{
		static const MeshDimType MESH_DIM_TYPE = MeshDimType::SPACE_3;
		typedef Vector3 VectorType;
		typedef Vector3 PointType;
		typedef Vector4 MatrixVectorType;
		typedef Matrix4x4 MatrixType;
		typedef Quaternion RotationType;
	};

	template<>
    const SpaceTypeInfo<SpaceType::SPACE_2>::VectorType SpaceConstInfo<SpaceType::SPACE_2>::UP_VECTOR = { 0.0f, 1.0f };

	template<>
	const SpaceTypeInfo<SpaceType::SPACE_3>::VectorType SpaceConstInfo<SpaceType::SPACE_3>::UP_VECTOR = { 0.0f, 1.0f, 0.0f };
} //namespace kgs

#endif // !VG_SCENE_OPTION_H
