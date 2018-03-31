#ifndef SAMPLES_LIB_SCENE_ASSIMP_H
#define SAMPLES_LIB_SCENE_ASSIMP_H

#include <framework/framework.hpp>

namespace sampleslib
{
    class AssimpScene
    {
    public:
        enum class VertexLayoutComponent {
            VERTEX_COMPONENT_POSITION = 0x0,
		    VERTEX_COMPONENT_NORMAL = 0x1,
		    VERTEX_COMPONENT_COLOR = 0x2,
		    VERTEX_COMPONENT_UV = 0x3,
		    VERTEX_COMPONENT_TANGENT = 0x4,
		    VERTEX_COMPONENT_BITANGENT = 0x5,
		    VERTEX_COMPONENT_DUMMY_FLOAT = 0x6,
		    VERTEX_COMPONENT_DUMMY_VEC4 = 0x7
        };

        struct CreateInfo {
        public:
            const char *fileName;
            uint32_t layoutComponentCount;
            const VertexLayoutComponent *pLayoutComponent;
            vg::Vector3 offset;
            vg::Vector3 scale;
            vg::Vector2 uvScale;
            vg::Bool32 isCreateObject;
            vg::Bool32 isRightHand;
            vg::Bool32 separateMesh;
            CreateInfo(const char* fileName = nullptr
                , uint32_t layoutComponentCount = 0u
                , const VertexLayoutComponent *pLayoutComponent = nullptr
                , vg::Vector3 offset = vg::Vector3(0.0f)
                , vg::Vector3 scale = vg::Vector3(1.0f)
                , vg::Vector2 uvScale = vg::Vector2(1.0f)
                , vg::Bool32 isCreateObject = VG_FALSE
                , vg::Bool32 isRightHand = VG_FALSE
                , vg::Bool32 separateMesh = VG_FALSE
                );
        };

        AssimpScene();
        AssimpScene(const CreateInfo &createInfo);
        void init(const CreateInfo &createInfo);
        const std::vector<std::shared_ptr<vg::DimSharedContentMesh3>> getMeshes() const;
        const std::vector<std::shared_ptr<vg::VisualObject3>> getObjects() const;
    protected:
        void _init(const CreateInfo &createInfo);
        std::shared_ptr<vg::VertexData> m_pSharedVertexData;
		std::shared_ptr<vg::IndexData> m_pSharedIndexData;
        std::vector<std::shared_ptr<vg::DimSharedContentMesh3>> m_pMeshes;
        std::vector<std::shared_ptr<vg::VisualObject3>> m_pObjects;
    private:

    };
}

#endif //SAMPLES_LIB_SCENE_ASSIMP_H