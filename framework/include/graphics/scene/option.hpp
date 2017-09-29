#ifndef KGS_SCENE_OPTION_H
#define KGS_SCENE_OPTION_H

#include "graphics/global.hpp"
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
		typedef void VectorType;
		typedef void PointType;
		typedef void MatrixVectorType;
		typedef void MatrixType;
		typedef void RotationType;
	};

	template<SpaceType type>
	struct SpaceConstInfo
	{
		const static MeshType MESH_TYPE;
		const static SpaceTypeInfo<type>::VectorType UP_VECTOR;
	};

	template<>
	struct SpaceTypeInfo<SpaceType::SPACE_2>
	{
		typedef Vector2 VectorType;
		typedef Vector2 PointType;
		typedef Vector3 MatrixVectorType;
		typedef Matrix3x3 MatrixType;
		typedef float RotationType;
	};

	template<>
	struct SpaceTypeInfo<SpaceType::SPACE_3>
	{
		typedef Vector3 VectorType;
		typedef Vector3 PointType;
		typedef Vector4 MatrixVectorType;
		typedef Matrix4x4 MatrixType;
		typedef Quaternion RotationType;
	};
} //namespace kgs

#endif // !KGS_SCENE_OPTION_H
