#ifndef VG_VISUAL_OBJECT_H
#define VG_VISUAL_OBJECT_H

#include <typeinfo>
#include <typeindex>
#include "graphics/scene/space_info.hpp"
#include "graphics/scene/object.hpp"
#include "graphics/mesh/mesh.hpp"
#include "graphics/material/material.hpp"

namespace vg
{
    class BaseVisualObject : public Base
    {
    public:
        struct BindInfo 
        {
            uint32_t framebufferWidth;
            uint32_t framebufferHeight;
            const Matrix4x4 *pProjMatrix;
            const Matrix4x4 *pViewMatrix;
            Bool32 hasClipRect;
            std::vector<fd::Rect2D> clipRects;
            BindInfo(uint32_t framebufferWidth = 0u
                , uint32_t framebufferHeight = 0u
                , const Matrix4x4 *pProjMatrix = nullptr
                , const Matrix4x4 *pViewMatrix = nullptr
                , Bool32 hasClipRect = VG_FALSE
                , std::vector<fd::Rect2D> clipRects = std::vector<fd::Rect2D>()
                );
        };
    
        struct BindResult
        {
           CmdBuffer *pBranchCmdBuffer;
           CmdBuffer *pTrunkRenderPassCmdBuffer;
           CmdBuffer *pTrunkWaitBarrierCmdBuffer;
           BindResult(CmdBuffer *pBranchCmdBuffer = nullptr
               , CmdBuffer *pTrunkRenderPassCmdBuffer = nullptr
               , CmdBuffer *pTrunkWaitBarrierCmdBuffer = nullptr
               );
        };

        BaseVisualObject();

        virtual ~BaseVisualObject();

        uint32_t getMaterialCount() const;
        void setMaterialCount(uint32_t count);

        const Material *getMaterial(uint32_t index = 0u) const;
        Material *getMaterial(uint32_t index = 0);
        void setMaterial(fd::ArrayProxy<Material *> pMaterials, uint32_t offset = 0u);
        void setMaterial(Material *pMaterial);

        const Material *getPreDepthMaterial(uint32_t index = 0u) const;
        Material *getPreDepthMaterial(uint32_t index = 0);
        void setPreDepthMaterial(fd::ArrayProxy<Material *> pMaterials, uint32_t offset = 0u);
        void setPreDepthMaterial(Material *pMaterial);

        const Material *getLightingMaterial(const std::type_info &lightTypeInfo, uint32_t index = 0u) const;
        Material *getLightingMaterial(const std::type_info &lightTypeInfo, uint32_t index = 0);
        void setLightingMaterial(const std::type_info &lightTypeInfo, fd::ArrayProxy<Material *> pMaterials, uint32_t offset = 0u);
        void setLightingMaterial(const std::type_info &lightTypeInfo, Material *pMaterial);

        const BaseMesh *getMesh() const;
        BaseMesh *getMesh();

        uint32_t getSubMeshOffset() const;
        uint32_t getSubMeshCount() const;

        void updateSubMeshInfo(uint32_t subMeshOffset, uint32_t subMeshCount);

        Bool32 getIsVisibilityCheck() const;
        void setIsVisibilityCheck(Bool32 value);

        // Bool32 getHasClipRect() const;
        // void setHasClipRect(Bool32 value);
        // uint32_t getClipRectCount() const;
        // const fd::Rect2D *getClipRects() const;
        // void updateClipRects(fd::ArrayProxy<fd::Rect2D> rects, uint32_t offset = 0u);
        // void updateClipRects(fd::Rect2D rect, uint32_t count, uint32_t offset = 0u);

        void beginBindForPreDepth(const BindInfo info, BindResult *pResult) const;
        void endBindForPreDepth() const;
        void beginBind(const BindInfo info, BindResult *pResult) const;
        void endBind() const;
        void beginBindForLighting(const std::type_info &lightTypeInfo, const BindInfo info, BindResult *pResult) const;
        void endBindForLighting(const std::type_info &lightTypeInfo) const;

    protected:
        uint32_t m_materialCount;
        std::vector<Material *> m_pMaterials;
        std::vector<Material *> m_pPreDepthMaterials;
        std::unordered_map<std::type_index, std::vector<Material *>> m_mapPLightingMaterials;

        BaseMesh *m_pMesh;
        int32_t m_subMeshOffset;
        int32_t m_subMeshCount;
        Bool32 m_isVisibilityCheck;
        // Bool32 m_hasClipRect;
        //Valid range of ClipRect is [(0, 0), (1, 1)]
        // std::vector<fd::Rect2D> m_clipRects;
        // void _asyncMeshData();
        void _resizeLightingMaterialMap();
        virtual Matrix4x4 _getModelMatrix() const = 0;

        void _checkPreDepthMaterialValid(uint32_t index) const;
        void _checkLightingMaterialValid(const std::type_info &lightTypeInfo, uint32_t index) const;
        void _checkMaterialValid(uint32_t index) const;
    };

    template <SpaceType SPACE_TYPE>
    class VisualObject : public BaseVisualObject, public Object<SPACE_TYPE>
    {
    public:
        using MeshDimType = Mesh<SpaceTypeInfo<SPACE_TYPE>::MESH_DIM_TYPE>;

        VisualObject()
            : BaseVisualObject()
            , Object<SPACE_TYPE>()
        {
            m_objectType = ObjectType::VISUAL_OBJECT;

        }

        void setMesh(MeshDimType *pMesh)
        {
            m_pMesh = pMesh;
            m_subMeshOffset = -1;
            m_subMeshCount = -1;
            //m_clipRects.resize(dynamic_cast<const ContentMesh *>(m_pMesh)->getSubMeshOffset());
        }

        void setMesh(MeshDimType *pMesh
            , uint32_t subMeshOffset
            , uint32_t subMeshCount)
        {
            m_pMesh = pMesh;
            m_subMeshOffset = subMeshOffset;
            m_subMeshCount = subMeshCount;
            m_clipRects.resize(subMeshCount);
        }
    protected:
        //aggregations
        
    };
} //namespace kgs

#endif // !VG_VISUAL_OBJECT_H
