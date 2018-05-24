#include "graphics/mesh/mesh_2.hpp"

namespace vg
{
	DimSepMesh2::DimSepMesh2(vk::MemoryPropertyFlags bufferMemoryPropertyFlags)
		: DimSepMesh<MeshDimType::SPACE_2>(bufferMemoryPropertyFlags)
	{
	}

	DimSimpleMesh2::DimSimpleMesh2(vk::MemoryPropertyFlags bufferMemoryPropertyFlags)
		: DimSimpleMesh<MeshDimType::SPACE_2>(bufferMemoryPropertyFlags)
	{
	}

	DimSharedContentMesh2::DimSharedContentMesh2()
	    : DimSharedContentMesh<MeshDimType::SPACE_2>()
	{

	}

	DimSharedContentMesh2::DimSharedContentMesh2(std::shared_ptr<VertexData> pVertexData
		    , std::shared_ptr<IndexData> pIndexData
			, uint32_t subIndexDataOffset
			, uint32_t subIndexDataCount
			)
			: DimSharedContentMesh<MeshDimType::SPACE_2>(
				pVertexData,
				pIndexData,
				subIndexDataOffset,
				subIndexDataCount
			)
	{

	}
} //namespace kgs