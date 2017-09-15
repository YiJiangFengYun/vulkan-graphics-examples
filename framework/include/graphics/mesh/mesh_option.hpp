#ifndef KGS_MESH_OPTION_H
#define KGS_MESH_OPTION_H

#include <array>
#include <vulkan/vulkan.hpp>
#include "graphics/global.hpp"

namespace kgs
{
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
