#ifndef VG_MESH_2_H

#include "graphics/mesh/mesh.hpp"
namespace vg
{
	class DimSepMesh2 : public DimSepMesh<MeshDimType::SPACE_2>
	{
	public:
		DimSepMesh2();

	private:

	};

	class DimSimpleMesh2 : public DimSimpleMesh<MeshDimType::SPACE_2>
	{
	public:
		DimSimpleMesh2();

	private:

	};
}

#endif //VG_MESH_2_H