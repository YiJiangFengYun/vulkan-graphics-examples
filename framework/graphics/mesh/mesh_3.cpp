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
} //namespace kgs