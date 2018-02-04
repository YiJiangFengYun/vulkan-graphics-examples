#ifndef VG_MESH_3_H

#include "graphics/mesh/mesh.hpp"
namespace vg
{
	class DimSepMesh3 : public DimSepMesh<MeshDimType::SPACE_3>
	{
	public:
		DimSepMesh3();
		DimSepMesh3(MemoryPropertyFlags bufferMemoryPropertyFlags);
	private:

	};

	class DimSimpleMesh3 : public DimSimpleMesh<MeshDimType::SPACE_3>
	{
	public:
		DimSimpleMesh3();
		DimSimpleMesh3(MemoryPropertyFlags bufferMemoryPropertyFlags);
	private:

	};
}

#endif //VG_MESH_2_H