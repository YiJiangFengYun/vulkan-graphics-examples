#ifndef VG_MESH_2_H
#define VG_MESH_2_H

#include "graphics/mesh/mesh.hpp"
namespace vg
{
	class DimSepMesh2 : public DimSepMesh<MeshDimType::SPACE_2>
	{
	public:
		DimSepMesh2(vk::MemoryPropertyFlags bufferMemoryPropertyFlags = vk::MemoryPropertyFlagBits::eDeviceLocal);

	private:

	};

	class DimSimpleMesh2 : public DimSimpleMesh<MeshDimType::SPACE_2>
	{
	public:
		DimSimpleMesh2(vk::MemoryPropertyFlags bufferMemoryPropertyFlags = vk::MemoryPropertyFlagBits::eDeviceLocal);

	private:

	};

	class DimSharedContentMesh2 : public DimSharedContentMesh<MeshDimType::SPACE_2>
	{
	public:
		DimSharedContentMesh2();
		DimSharedContentMesh2(std::shared_ptr<VertexData> pVertexData
		    , std::shared_ptr<IndexData> pIndexData
			, uint32_t subIndexDataOffset
			, uint32_t subIndexDataCount
			);

	private:

	};
}

#endif //VG_MESH_2_H