#ifndef VG_MESH_3_H

#include "graphics/mesh/mesh.hpp"
namespace vg
{
    class DimSepMesh3 : public DimSepMesh<MeshDimType::SPACE_3>
    {
    public:
        DimSepMesh3(vk::MemoryPropertyFlags bufferMemoryPropertyFlags = vk::MemoryPropertyFlagBits::eDeviceLocal);
    private:

    };

    class DimSimpleMesh3 : public DimSimpleMesh<MeshDimType::SPACE_3>
    {
    public:
        DimSimpleMesh3(vk::MemoryPropertyFlags bufferMemoryPropertyFlags = vk::MemoryPropertyFlagBits::eDeviceLocal);
    private:

    };

    class DimSharedContentMesh3 : public DimSharedContentMesh<MeshDimType::SPACE_3>
    {
    public:
        DimSharedContentMesh3();
        DimSharedContentMesh3(std::shared_ptr<VertexData> pVertexData
            , std::shared_ptr<IndexData> pIndexData
            , uint32_t subIndexDataOffset
            , uint32_t subIndexDataCount
            );
    private:

    };
}

#endif //VG_MESH_2_H