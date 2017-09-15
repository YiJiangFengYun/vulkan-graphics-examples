#include "graphics/mesh/mesh_option.hpp"

namespace kgs
{
	std::array<std::pair<PrimitiveTopology, vk::PrimitiveTopology>, static_cast<size_t>(PrimitiveTopology::RANGE_SIZE)> arrPrimitiveTopologyToVK = {
		std::pair<PrimitiveTopology, vk::PrimitiveTopology>(PrimitiveTopology::POINT_LIST, vk::PrimitiveTopology::ePointList),
		std::pair<PrimitiveTopology, vk::PrimitiveTopology>(PrimitiveTopology::LINE_LIST, vk::PrimitiveTopology::eLineList),
		std::pair<PrimitiveTopology, vk::PrimitiveTopology>(PrimitiveTopology::LINE_STRIP, vk::PrimitiveTopology::eLineStrip),
		std::pair<PrimitiveTopology, vk::PrimitiveTopology>(PrimitiveTopology::TRIANGLE_LIST, vk::PrimitiveTopology::eTriangleList),
		std::pair<PrimitiveTopology, vk::PrimitiveTopology>(PrimitiveTopology::TRIANGLE_STRIP, vk::PrimitiveTopology::eTriangleStrip)
	}
} //namespace kgs