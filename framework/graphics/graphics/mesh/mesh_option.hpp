#ifndef VG_MESH_OPTION_H
#define VG_MESH_OPTION_H

#include <array>
#include <vulkan/vulkan.hpp>
#include "graphics/global.hpp"
#include "graphics/mesh/mesh_data.hpp"

namespace vg
{
    enum class MeshDimType
    {
        SPACE_2,
        SPACE_3,
        BEGIN_RANGE = SPACE_2,
        END_RANGE = SPACE_3,
        RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
    };

    template<MeshDimType type>
    struct MeshTypeInfo
    {
        using PointType = void;
    };

    template<MeshDimType type>
    struct MeshConstInfo
    {
        static const MeshData::DataType ARRAY_TYPE;
    };

    enum class TextureCoordinateType
    {
        FLOAT,
        VECTOR_2,
        VECTOR_3,
        BEGIN_RANGE = FLOAT,
        END_RANGE = VECTOR_3,
        RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
    };

    template<TextureCoordinateType type>
    struct TextureCoordinateConstInfo
    {
        static const MeshData::DataType BASE_TYPE;
        static const MeshData::DataType ARRAY_TYPE;
    };

    enum class TextureCoordinateIndex
    {
        TextureCoordinate_0,
        TextureCoordinate_1,
        TextureCoordinate_2,
        TextureCoordinate_3
    };

    template<TextureCoordinateIndex index>
    struct TextureCoordinateIndexInfo
    {
        static const std::string VERTEX_NAME;
        static const std::uint32_t VERTEX_BINDING_PRIORITY;
    };

    enum class PrimitiveTopology
    {
        POINT_LIST = 0,
        LINE_LIST = 1,
        LINE_STRIP = 2,
        TRIANGLE_LIST = 3,
        TRIANGLE_STRIP = 4,
        BEGIN_RANGE = POINT_LIST,
        END_RANGE = TRIANGLE_STRIP,
        RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
    };

    extern std::array<std::pair<PrimitiveTopology, vk::PrimitiveTopology>, static_cast<size_t>(PrimitiveTopology::RANGE_SIZE)> arrPrimitiveTopologyToVK;

    inline vk::PrimitiveTopology tranPrimitiveTopologyTypeToVK(PrimitiveTopology topology)
    {
        vk::PrimitiveTopology vkTopology;
#ifdef DEBUG
        Bool32 isHave = VG_FALSE;
#endif // DEBUG
        for (const auto& item2 : arrPrimitiveTopologyToVK)
        {
            if (item2.first == topology)
            {
                vkTopology = item2.second;
#ifdef DEBUG
                isHave = VG_TRUE;
#endif // DEBUG
                break;
            }
        }

#ifdef DEBUG
        if (isHave == VG_FALSE)
        {
            throw std::runtime_error("Invalid primitive topology.");
        }
#endif // DEBUG
        return vkTopology;
    }


    //-------specialization--------------------------------------------------------------------------

    template<>
    struct MeshTypeInfo<MeshDimType::SPACE_2>
    {
        using PointType = Vector2;
    };

    template<>
    struct MeshTypeInfo<MeshDimType::SPACE_3>
    {
        using PointType = Vector3;
    };

    // Using struct declaration type specialization, so these members can't be used to indicate innter type of class when declarated.
    // Otherwise, these member value will not be definited when class declaration.
    template<>
    struct MeshConstInfo<MeshDimType::SPACE_2>
    {
        static const MeshData::DataType ARRAY_TYPE = MeshData::DataType::VECTOR_2_ARRAY;
    };

    template<>
    struct MeshConstInfo<MeshDimType::SPACE_3>
    {
        static const MeshData::DataType ARRAY_TYPE = MeshData::DataType::VECTOR_3_ARRAY;
    };

    template<>
    struct TextureCoordinateConstInfo<TextureCoordinateType::FLOAT>
    {
        static const MeshData::DataType ARRAY_TYPE = MeshData::DataType::FLOAT_ARRAY;
    };

    template<>
    struct TextureCoordinateConstInfo<TextureCoordinateType::VECTOR_2>
    {
        static const MeshData::DataType ARRAY_TYPE = MeshData::DataType::VECTOR_2_ARRAY;
    };

    template<>
    struct TextureCoordinateConstInfo<TextureCoordinateType::VECTOR_3>
    {
        static const MeshData::DataType ARRAY_TYPE = MeshData::DataType::VECTOR_3_ARRAY;
    };

    template<>
    const std::string TextureCoordinateIndexInfo<TextureCoordinateIndex::TextureCoordinate_0>::VERTEX_NAME = VG_VERTEX_TextureCoordinate0_NAME;

    template<>
    const std::uint32_t TextureCoordinateIndexInfo<TextureCoordinateIndex::TextureCoordinate_0>::VERTEX_BINDING_PRIORITY = VG_VERTEX_BINDING_PRIORITY_TextureCoordinate0;

    template<>
    const std::string TextureCoordinateIndexInfo<TextureCoordinateIndex::TextureCoordinate_1>::VERTEX_NAME = VG_VERTEX_TextureCoordinate1_NAME;

    template<>
    const std::uint32_t TextureCoordinateIndexInfo<TextureCoordinateIndex::TextureCoordinate_1>::VERTEX_BINDING_PRIORITY = VG_VERTEX_BINDING_PRIORITY_TextureCoordinate1;

    template<>
    const std::string TextureCoordinateIndexInfo<TextureCoordinateIndex::TextureCoordinate_2>::VERTEX_NAME = VG_VERTEX_TextureCoordinate2_NAME;

    template<>
    const std::uint32_t TextureCoordinateIndexInfo<TextureCoordinateIndex::TextureCoordinate_2>::VERTEX_BINDING_PRIORITY = VG_VERTEX_BINDING_PRIORITY_TextureCoordinate2;

    template<>
    const std::string TextureCoordinateIndexInfo<TextureCoordinateIndex::TextureCoordinate_3>::VERTEX_NAME = VG_VERTEX_TextureCoordinate3_NAME;

    template<>
    const std::uint32_t TextureCoordinateIndexInfo<TextureCoordinateIndex::TextureCoordinate_3>::VERTEX_BINDING_PRIORITY = VG_VERTEX_BINDING_PRIORITY_TextureCoordinate3;
} //namespace kgs

#endif // !VG_MESH_OPTION_H
