#include "graphics/mesh/mesh_2.hpp"

namespace vg
{
	DimSepMesh2::DimSepMesh2()
		: DimSepMesh<MeshDimType::SPACE_2>()
	{
	}

	DimSepMesh2::DimSepMesh2(MemoryPropertyFlags bufferMemoryPropertyFlags)
		: DimSepMesh<MeshDimType::SPACE_2>(bufferMemoryPropertyFlags)
	{
	}

	DimSimpleMesh2::DimSimpleMesh2()
		: DimSimpleMesh<MeshDimType::SPACE_2>()
	{
	}

	DimSimpleMesh2::DimSimpleMesh2(MemoryPropertyFlags bufferMemoryPropertyFlags)
		: DimSimpleMesh<MeshDimType::SPACE_2>(bufferMemoryPropertyFlags)
	{
	}
} //namespace kgs