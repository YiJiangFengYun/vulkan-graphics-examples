#ifndef KGS_MESH_OPTION_H
#define KGS_MESH_OPTION_H

#include <array>
#include <vulkan/vulkan.hpp>
#include "graphics/global.hpp"
#include "graphics/mesh/mesh_data.hpp"

namespace kgs
{
	enum class MeshType
	{
		SPACE_2,
		SPACE_3
	};

	template<MeshType type>
	struct MeshTypeInfo
	{
		typedef Vector3 PointType;
	};

	template<MeshType type>
	struct MeshConstInfo
	{
		static const MeshData::DataType ARRAY_TYPE;
	};

	enum class UVType
	{
		FLOAT,
		VECTOR_2,
		VECTOR_3,
		BEGIN_RANGE = FLOAT,
		END_RANGE = VECTOR_3,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};

	template<UVType type>
	struct UVConstInfo
	{
		static const MeshData::DataType BASE_TYPE;
		static const MeshData::DataType ARRAY_TYPE;
	};

	enum class UVIndex
	{
		UV_0,
		UV_1,
		UV_2,
		UV_3
	};

#define KGS_VERTEX_POSITION_NAME "_Position"
#define KGS_VERTEX_COLOR_NAME "_Color"
#define KGS_VERTEX_NORMAL_NAME "_Normal"
#define KGS_VERTEX_TANGENT_NAME "_Tangent"
#define KGS_VERTEX_UV0_NAME "_UV_0"
#define KGS_VERTEX_UV1_NAME "_UV_1"
#define KGS_VERTEX_UV2_NAME "_UV_2"
#define KGS_VERTEX_UV3_NAME "_UV_3"

#define KGS_VERTEX_BINDING_PRIORITY_POSITION 0u
#define KGS_VERTEX_BINDING_PRIORITY_COLOR 1u
#define KGS_VERTEX_BINDING_PRIORITY_NORMAL 2u
#define KGS_VERTEX_BINDING_PRIORITY_TANGENT 3u
#define KGS_VERTEX_BINDING_PRIORITY_UV0 4u
#define KGS_VERTEX_BINDING_PRIORITY_UV1 5u
#define KGS_VERTEX_BINDING_PRIORITY_UV2 6u
#define KGS_VERTEX_BINDING_PRIORITY_UV3 7u
#define KGS_VERTEX_BINDING_PRIORITY_OTHER_MIN 8u

	template<UVIndex index>
	struct UVIndexInfo
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

	inline vk::PrimitiveTopology tranDescriptorTypeToVK(PrimitiveTopology topology)
	{
		vk::PrimitiveTopology vkTopology;
#ifdef DEBUG
		Bool32 isHave = KGS_FALSE;
#endif // DEBUG
		for (const auto& item2 : arrPrimitiveTopologyToVK)
		{
			if (item2.first == topology)
			{
				vkTopology = item2.second;
#ifdef DEBUG
				isHave = KGS_TRUE;
#endif // DEBUG
				break;
			}
		}

#ifdef DEBUG
		if (isHave == KGS_FALSE)
		{
			throw std::runtime_error("Invalid primitive topology.");
		}
#endif // DEBUG
		return vkTopology;
	}


	//-------specialization--------------------------------------------------------------------------
	template<>
	const MeshData::DataType MeshConstInfo<MeshType::SPACE_2>::ARRAY_TYPE = MeshData::DataType::VECTOR_2_ARRAY;

	template<>
	const MeshData::DataType MeshConstInfo<MeshType::SPACE_3>::ARRAY_TYPE = MeshData::DataType::VECTOR_3_ARRAY;

	template<>
	const MeshData::DataType UVConstInfo<UVType::FLOAT>::ARRAY_TYPE = MeshData::DataType::FLOAT_ARRAY;

	template<>
	const MeshData::DataType UVConstInfo<UVType::VECTOR_2>::ARRAY_TYPE = MeshData::DataType::VECTOR_2_ARRAY;

	template<>
	const MeshData::DataType UVConstInfo<UVType::VECTOR_3>::ARRAY_TYPE = MeshData::DataType::VECTOR_3_ARRAY;

	template<>
	const std::string UVIndexInfo<UVIndex::UV_0>::VERTEX_NAME = KGS_VERTEX_UV0_NAME;

	template<>
	const std::uint32_t UVIndexInfo<UVIndex::UV_0>::VERTEX_BINDING_PRIORITY = KGS_VERTEX_BINDING_PRIORITY_UV0;

	template<>
	const std::string UVIndexInfo<UVIndex::UV_1>::VERTEX_NAME = KGS_VERTEX_UV1_NAME;

	template<>
	const std::uint32_t UVIndexInfo<UVIndex::UV_1>::VERTEX_BINDING_PRIORITY = KGS_VERTEX_BINDING_PRIORITY_UV1;

	template<>
	const std::string UVIndexInfo<UVIndex::UV_2>::VERTEX_NAME = KGS_VERTEX_UV2_NAME;

	template<>
	const std::uint32_t UVIndexInfo<UVIndex::UV_2>::VERTEX_BINDING_PRIORITY = KGS_VERTEX_BINDING_PRIORITY_UV2;

	template<>
	const std::string UVIndexInfo<UVIndex::UV_3>::VERTEX_NAME = KGS_VERTEX_UV3_NAME;

	template<>
	const std::uint32_t UVIndexInfo<UVIndex::UV_3>::VERTEX_BINDING_PRIORITY = KGS_VERTEX_BINDING_PRIORITY_UV3;
} //namespace kgs

#endif // !KGS_MESH_OPTION_H
