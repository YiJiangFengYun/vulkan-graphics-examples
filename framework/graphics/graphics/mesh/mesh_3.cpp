#include "graphics/mesh/mesh_3.hpp"

namespace vg
{
	DimSepMesh3::DimSepMesh3()
		: DimSepMesh<MeshDimType::SPACE_3>()
	{
	}

	DimSepMesh3::DimSepMesh3(MemoryPropertyFlags bufferMemoryPropertyFlags)
		: DimSepMesh<MeshDimType::SPACE_3>(bufferMemoryPropertyFlags)
	{
	}

	DimSimpleMesh3::DimSimpleMesh3()
		: DimSimpleMesh<MeshDimType::SPACE_3>()
	{
	}

	DimSimpleMesh3::DimSimpleMesh3(MemoryPropertyFlags bufferMemoryPropertyFlags)
		: DimSimpleMesh<MeshDimType::SPACE_3>(bufferMemoryPropertyFlags)
	{
	}

	DimSharedContentMesh3::DimSharedContentMesh3()
	    : DimSharedContentMesh<MeshDimType::SPACE_3>()
    {

	}
	
	DimSharedContentMesh3::DimSharedContentMesh3(std::shared_ptr<VertexData> pVertexData
		    , std::shared_ptr<IndexData> pIndexData
			, uint32_t subIndexDataOffset
			, uint32_t subIndexDataCount
			)
			: DimSharedContentMesh<MeshDimType::SPACE_3>(
				pVertexData,
				pIndexData,
				subIndexDataOffset,
				subIndexDataCount
			)
	{

	}
} //namespace kgs