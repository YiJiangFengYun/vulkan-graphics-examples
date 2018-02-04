#ifndef VG_MESH_2_H

#include "graphics/mesh/mesh.hpp"
namespace vg
{
	class DimSepMesh2 : public DimSepMesh<MeshDimType::SPACE_2>
	{
	public:
		DimSepMesh2();
		DimSepMesh2(MemoryPropertyFlags bufferMemoryPropertyFlags);

	private:

	};

	class DimSimpleMesh2 : public DimSimpleMesh<MeshDimType::SPACE_2>
	{
	public:
		DimSimpleMesh2();
		DimSimpleMesh2(MemoryPropertyFlags bufferMemoryPropertyFlags);

	private:

	};
}

#endif //VG_MESH_2_H