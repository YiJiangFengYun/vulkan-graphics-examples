#ifndef SAMPLES_LIB_MESH_ASSIMP_H
#define SAMPLES_LIB_MESH_ASSIMP_H

#include <framework/framework.hpp>

namespace sampleslib
{
    class AssimpMesh : public vg::ContentMesh
    {
    public:

    protected:

    private:

    };

    template <vg::MeshDimType meshDimType>
    class DimAssimpMesh : public AssimpMesh, public Mesh<meshDimType>
    {
    public:

    protected:

    private:

    };

    class AssimpMesh3 : public DimAssimpMesh<vg::MeshDimType::SPACE_3>
    {
    public:

    private:
    };

    class AssimpMesh2 : public DimAssimpMesh<vg::MeshDimType::SPACE_2>
    {
    public:

    private:
    };
}

#endif //SAMPLES_LIB_MESH_ASSIMP_H