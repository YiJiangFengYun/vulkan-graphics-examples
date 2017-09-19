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
		const static MeshData::DataType BASE_TYPE;
		const static MeshData::DataType ARRAY_TYPE;
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
	struct UVTypeInfo
	{
		const static MeshData::DataType BASE_TYPE;
		const static MeshData::DataType ARRAY_TYPE;
	};

	enum class UVIndex
	{
		UV_0,
		UV_1,
		UV_2,
	};

#define KGS_VERTEX_POSTION_NAME "_Position"
#define KGS_VERTEX_COLOR_NAME "_Color"
#define KGS_VERTEX_NORMAL_NAME "_Normal"
#define KGS_VERTEX_TANGENT_NAME "_Tangent"
	//#define KGS_VERTEX_UV

#define KGS_VERTEX_POSITION_LOCATION 0u
#define KGS_VERTEX_COLOR_LOCATION 1u
#define KGS_VERTEX_NORMAL_LOCATION 2u
#define KGS_VERTEX_TANGENT_LOCATION 3u

	template<UVIndex index>
	struct UVIndexInfo
	{
		static const std::string VERTEX_NAME;
		static const std::uint32_t VERTEX_LOCATION;
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
} //namespace kgs

#endif // !KGS_MESH_OPTION_H
