#include "graphics/mesh/mesh_option.hpp"

namespace kgs
{
	template<>
	const MeshData::DataType MeshTypeInfo<MeshType::SPACE_2>::BASE_TYPE = MeshData::DataType::VECTOR_2;

	template<>
	const MeshData::DataType MeshTypeInfo<MeshType::SPACE_2>::ARRAY_TYPE = MeshData::DataType::VECTOR_2_ARRAY;

	template<>
	const MeshData::DataType MeshTypeInfo<MeshType::SPACE_3>::BASE_TYPE = MeshData::DataType::VECTOR_3;

	template<>
	const MeshData::DataType MeshTypeInfo<MeshType::SPACE_3>::ARRAY_TYPE = MeshData::DataType::VECTOR_3_ARRAY;

	template<>
	const MeshData::DataType UVTypeInfo<UVType::FLOAT>::BASE_TYPE = MeshData::DataType::FLOAT;

	template<>
	const MeshData::DataType UVTypeInfo<UVType::FLOAT>::ARRAY_TYPE = MeshData::DataType::FLOAT_ARRAY;

	template<>
	const MeshData::DataType UVTypeInfo<UVType::VECTOR_2>::BASE_TYPE = MeshData::DataType::VECTOR_2;

	template<>
	const MeshData::DataType UVTypeInfo<UVType::VECTOR_2>::ARRAY_TYPE = MeshData::DataType::VECTOR_2_ARRAY;

	template<>
	const MeshData::DataType UVTypeInfo<UVType::VECTOR_3>::BASE_TYPE = MeshData::DataType::VECTOR_3;

	template<>
	const MeshData::DataType UVTypeInfo<UVType::VECTOR_3>::ARRAY_TYPE = MeshData::DataType::VECTOR_3_ARRAY;

	template<>
	const std::string UVIndexInfo<UVIndex::UV_0>::VERTEX_NAME = KGS_VERTEX_UV0_NAME;

	template<>
	const std::uint32_t UVIndexInfo<UVIndex::UV_0>::VERTEX_BINDING = KGS_VERTEX_UV0_BINDING;

	template<>
	const std::string UVIndexInfo<UVIndex::UV_1>::VERTEX_NAME = KGS_VERTEX_UV1_NAME;

	template<>
	const std::uint32_t UVIndexInfo<UVIndex::UV_1>::VERTEX_BINDING = KGS_VERTEX_UV1_BINDING;

	template<>
	const std::string UVIndexInfo<UVIndex::UV_2>::VERTEX_NAME = KGS_VERTEX_UV2_NAME;

	template<>
	const std::uint32_t UVIndexInfo<UVIndex::UV_2>::VERTEX_BINDING = KGS_VERTEX_UV2_BINDING;

	template<>
	const std::string UVIndexInfo<UVIndex::UV_3>::VERTEX_NAME = KGS_VERTEX_UV3_NAME;

	template<>
	const std::uint32_t UVIndexInfo<UVIndex::UV_3>::VERTEX_BINDING = KGS_VERTEX_UV3_BINDING;


	std::array<std::pair<PrimitiveTopology, vk::PrimitiveTopology>, static_cast<size_t>(PrimitiveTopology::RANGE_SIZE)> arrPrimitiveTopologyToVK = {
		std::pair<PrimitiveTopology, vk::PrimitiveTopology>(PrimitiveTopology::POINT_LIST, vk::PrimitiveTopology::ePointList),
		std::pair<PrimitiveTopology, vk::PrimitiveTopology>(PrimitiveTopology::LINE_LIST, vk::PrimitiveTopology::eLineList),
		std::pair<PrimitiveTopology, vk::PrimitiveTopology>(PrimitiveTopology::LINE_STRIP, vk::PrimitiveTopology::eLineStrip),
		std::pair<PrimitiveTopology, vk::PrimitiveTopology>(PrimitiveTopology::TRIANGLE_LIST, vk::PrimitiveTopology::eTriangleList),
		std::pair<PrimitiveTopology, vk::PrimitiveTopology>(PrimitiveTopology::TRIANGLE_STRIP, vk::PrimitiveTopology::eTriangleStrip)
	};
} //namespace kgs