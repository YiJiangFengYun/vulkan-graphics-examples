#ifndef VG_MESH_OPTION_H
#define VG_MESH_OPTION_H

#include <array>
#include <vulkan/vulkan.hpp>
#include "graphics/global.hpp"
#include "graphics/mesh/mesh_data.hpp"

namespace vg
{
	enum class MeshType
	{
		SPACE_2,
		SPACE_3,
		BEGIN_RANGE = SPACE_2,
		END_RANGE = SPACE_3,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};

	template<MeshType type>
	struct MeshTypeInfo
	{
		typedef void PointType;
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
	struct MeshTypeInfo<MeshType::SPACE_2>
	{
		typedef Vector2 PointType;
	};

	template<>
	struct MeshTypeInfo<MeshType::SPACE_3>
	{
		typedef Vector3 PointType;
	};

	// Using struct declaration type specialization, so these members can't be used to indicate innter type of class when declarated.
	// Otherwise, these member value will not be definited when class declaration.
	template<>
	struct MeshConstInfo<MeshType::SPACE_2>
	{
		static const MeshData::DataType ARRAY_TYPE = MeshData::DataType::VECTOR_2_ARRAY;
	};

	template<>
	struct MeshConstInfo<MeshType::SPACE_3>
	{
		static const MeshData::DataType ARRAY_TYPE = MeshData::DataType::VECTOR_3_ARRAY;
	};

	template<>
	struct UVConstInfo<UVType::FLOAT>
	{
		static const MeshData::DataType ARRAY_TYPE = MeshData::DataType::FLOAT_ARRAY;
	};

	template<>
	struct UVConstInfo<UVType::VECTOR_2>
	{
		static const MeshData::DataType ARRAY_TYPE = MeshData::DataType::VECTOR_2_ARRAY;
	};

	template<>
	struct UVConstInfo<UVType::VECTOR_3>
	{
		static const MeshData::DataType ARRAY_TYPE = MeshData::DataType::VECTOR_3_ARRAY;
	};

	/*template<>
	const MeshData::DataType MeshConstInfo<MeshType::SPACE_2>::ARRAY_TYPE = MeshData::DataType::VECTOR_2_ARRAY;

	template<>
	const MeshData::DataType MeshConstInfo<MeshType::SPACE_3>::ARRAY_TYPE = MeshData::DataType::VECTOR_3_ARRAY;

	template<>
	const MeshData::DataType UVConstInfo<UVType::FLOAT>::ARRAY_TYPE = MeshData::DataType::FLOAT_ARRAY;

	template<>
	const MeshData::DataType UVConstInfo<UVType::VECTOR_2>::ARRAY_TYPE = MeshData::DataType::VECTOR_2_ARRAY;

	template<>
	const MeshData::DataType UVConstInfo<UVType::VECTOR_3>::ARRAY_TYPE = MeshData::DataType::VECTOR_3_ARRAY;*/

	template<>
	const std::string UVIndexInfo<UVIndex::UV_0>::VERTEX_NAME = VG_VERTEX_UV0_NAME;

	template<>
	const std::uint32_t UVIndexInfo<UVIndex::UV_0>::VERTEX_BINDING_PRIORITY = VG_VERTEX_BINDING_PRIORITY_UV0;

	template<>
	const std::string UVIndexInfo<UVIndex::UV_1>::VERTEX_NAME = VG_VERTEX_UV1_NAME;

	template<>
	const std::uint32_t UVIndexInfo<UVIndex::UV_1>::VERTEX_BINDING_PRIORITY = VG_VERTEX_BINDING_PRIORITY_UV1;

	template<>
	const std::string UVIndexInfo<UVIndex::UV_2>::VERTEX_NAME = VG_VERTEX_UV2_NAME;

	template<>
	const std::uint32_t UVIndexInfo<UVIndex::UV_2>::VERTEX_BINDING_PRIORITY = VG_VERTEX_BINDING_PRIORITY_UV2;

	template<>
	const std::string UVIndexInfo<UVIndex::UV_3>::VERTEX_NAME = VG_VERTEX_UV3_NAME;

	template<>
	const std::uint32_t UVIndexInfo<UVIndex::UV_3>::VERTEX_BINDING_PRIORITY = VG_VERTEX_BINDING_PRIORITY_UV3;
} //namespace kgs

#endif // !VG_MESH_OPTION_H
