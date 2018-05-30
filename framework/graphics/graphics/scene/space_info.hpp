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
        using VectorType = void;
        using PointType = void;
        using MatrixVectorType = void;
        using MatrixType = void;
        using RotationType = void;
        using RotationDimType = void;
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
        using VectorType = Vector2;
        using PointType = Vector2;
        using MatrixVectorType = Vector3;
        using MatrixType = Matrix3x3;
        using RotationType = float;
        using RotationDimType = float;
    };

    template<>
    struct SpaceTypeInfo<SpaceType::SPACE_3>
    {
        static const MeshDimType MESH_DIM_TYPE = MeshDimType::SPACE_3;
        using VectorType = Vector3;
        using PointType = Vector3;
        using MatrixVectorType = Vector4;
        using MatrixType = Matrix4x4;
        using RotationType = Quaternion;
        using RotationDimType = Vector3;
    };

    template<>
    const SpaceTypeInfo<SpaceType::SPACE_2>::VectorType SpaceConstInfo<SpaceType::SPACE_2>::UP_VECTOR = { 0.0f, 1.0f };

    template<>
    const SpaceTypeInfo<SpaceType::SPACE_3>::VectorType SpaceConstInfo<SpaceType::SPACE_3>::UP_VECTOR = { 0.0f, 1.0f, 0.0f };
} //namespace kgs

#endif // !VG_SCENE_OPTION_H
