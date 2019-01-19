#include "graphics/scene/visual_object.hpp"

namespace vg
{
    BaseVisualObject::BindInfo::BindInfo(uint32_t framebufferWidth
        , uint32_t framebufferHeight
        , const Matrix4x4 *pProjMatrix
        , const Matrix4x4 *pViewMatrix
        , Bool32 hasClipRect
        , std::vector<fd::Rect2D> clipRects
        )
        : framebufferWidth(framebufferWidth)
        , framebufferHeight(framebufferHeight)
        , pProjMatrix(pProjMatrix)
        , pViewMatrix(pViewMatrix)
        , hasClipRect(hasClipRect)
        , clipRects(clipRects)
    {
    }

    BaseVisualObject::BindResult::BindResult(CmdBuffer *pBranchCmdBuffer
        , CmdBuffer *pTrunkRenderPassCmdBuffer
        , CmdBuffer *pTrunkWaitBarrierCmdBuffer
        )
        : pBranchCmdBuffer(pBranchCmdBuffer)
        , pTrunkRenderPassCmdBuffer(pTrunkRenderPassCmdBuffer)
        , pTrunkWaitBarrierCmdBuffer(pTrunkWaitBarrierCmdBuffer)
    {

    }

    BaseVisualObject::BaseVisualObject()
        : Base(BaseType::SCENE_OBJECT)
        , m_materialCount(0)
        , m_pMaterials()
        , m_pPreDepthMaterials()
        , m_mapPLightingMaterials()
        , m_pMesh()
        , m_subMeshOffset(-1)
        , m_subMeshCount(-1)
        , m_isVisibilityCheck(VG_TRUE)
    {

    }

    BaseVisualObject::~BaseVisualObject()
    {

    }

    uint32_t BaseVisualObject::getMaterialCount() const
    {
        return m_materialCount;
    }

    void BaseVisualObject::setMaterialCount(uint32_t count)
    {
        if (m_materialCount != count) {
            m_materialCount = count;
            m_pMaterials.resize(count);
            m_pPreDepthMaterials.resize(count);
            _resizeLightingMaterialMap();
        }
    }

    const Material *BaseVisualObject::getMaterial(uint32_t index) const
    {
        _checkMaterialValid(index);
        return m_pMaterials[index];
    }

    Material *BaseVisualObject::getMaterial(uint32_t index)
    {
        _checkMaterialValid(index);
        return m_pMaterials[index];
    }

    void BaseVisualObject::setMaterial(fd::ArrayProxy<Material *> pMaterials, uint32_t offset)
    {
        uint32_t count = static_cast<uint32_t>(pMaterials.size());
        for (uint32_t i = 0; i < count; ++i)
        {
            m_pMaterials[offset] = *(pMaterials.data() + i);
            ++offset;
        }
    }

    void BaseVisualObject::setMaterial(Material * pMaterial)
    {
        for (uint32_t i = 0; i < m_materialCount; ++i) {
            m_pMaterials[i] = pMaterial;
        }
    }

    const Material *BaseVisualObject::getPreDepthMaterial(uint32_t index) const
    {
        _checkPreDepthMaterialValid(index);
        return m_pPreDepthMaterials[index];
    }
        
    Material *BaseVisualObject::getPreDepthMaterial(uint32_t index)
    {
        _checkPreDepthMaterialValid(index);
        return m_pPreDepthMaterials[index];
    }

    void BaseVisualObject::setPreDepthMaterial(fd::ArrayProxy<Material *> pMaterials, uint32_t offset)
    {
        uint32_t count = static_cast<uint32_t>(pMaterials.size());
        for (uint32_t i = 0; i < count; ++i)
        {
            m_pPreDepthMaterials[offset] = *(pMaterials.data() + i);
            ++offset;
        }
    }
        
    void BaseVisualObject::setPreDepthMaterial(Material *pMaterial)
    {
        for (uint32_t i = 0; i < m_materialCount; ++i) {
            m_pPreDepthMaterials[i] = pMaterial;
        }
    }

    const Material *BaseVisualObject::getLightingMaterial(const std::type_info &lightTypeInfo, uint32_t index) const
    {
        _checkLightingMaterialValid(lightTypeInfo, index);
        const auto &iterator = m_mapPLightingMaterials.find(std::type_index(lightTypeInfo));
        return iterator->second[index];
    }
        
    Material *BaseVisualObject::getLightingMaterial(const std::type_info &lightTypeInfo, uint32_t index)
    {
        _checkLightingMaterialValid(lightTypeInfo, index);
        return m_mapPLightingMaterials[std::type_index(lightTypeInfo)][index];
    }

    void BaseVisualObject::setLightingMaterial(const std::type_info &lightTypeInfo, fd::ArrayProxy<Material *> pMaterials, uint32_t offset)
    {
        if (m_mapPLightingMaterials.count(std::type_index(lightTypeInfo)) == 0) 
        {
            m_mapPLightingMaterials[std::type_index(lightTypeInfo)].resize(m_materialCount);
        }

        auto &pLightMaterials = m_mapPLightingMaterials[std::type_index(lightTypeInfo)];
        uint32_t count = static_cast<uint32_t>(pMaterials.size());
        for (uint32_t i = 0; i < count; ++i)
        {
            pLightMaterials[offset] = *(pMaterials.data() + i);
            ++offset;
        }
    }
        
    void BaseVisualObject::setLightingMaterial(const std::type_info &lightTypeInfo, Material *pMaterial)
    {
        if (m_mapPLightingMaterials.count(std::type_index(lightTypeInfo)) == 0) 
        {
            m_mapPLightingMaterials[std::type_index(lightTypeInfo)].resize(m_materialCount);
        }
        auto &pLightMaterials = m_mapPLightingMaterials[std::type_index(lightTypeInfo)];
        for (uint32_t i = 0; i < m_materialCount; ++i) {
            pLightMaterials[i] = pMaterial;
        }
    }

    const BaseMesh *BaseVisualObject::getMesh() const
    {
        return m_pMesh;
    }

    BaseMesh *BaseVisualObject::getMesh()
    {
        return m_pMesh;
    }

    uint32_t BaseVisualObject::getSubMeshOffset() const
    {
        if (m_subMeshOffset < 0) {
            return dynamic_cast<const ContentMesh *>(m_pMesh)->getSubMeshOffset();
        } else {
            return m_subMeshOffset;
        }
    }
        
    uint32_t BaseVisualObject::getSubMeshCount() const
    {
        if (m_subMeshCount < 0) {
            return dynamic_cast<const ContentMesh *>(m_pMesh)->getSubMeshCount();
        } else {
            return m_subMeshCount;
        }
    }

    void BaseVisualObject::updateSubMeshInfo(uint32_t subMeshOffset, uint32_t subMeshCount)
    {
        m_subMeshOffset = subMeshOffset;
        m_subMeshCount = static_cast<int32_t>(subMeshCount);
    }

    Bool32 BaseVisualObject::getIsVisibilityCheck() const
    {
        return m_isVisibilityCheck;
    }

    void BaseVisualObject::setIsVisibilityCheck(Bool32 value)
    {
        m_isVisibilityCheck = value;
    }

    void BaseVisualObject::beginBindForPreDepth(const BindInfo info, BindResult *pResult) const
    {
        auto &result = *pResult;
        uint32_t subMeshCount = getSubMeshCount();
        auto modelMatrix = _getModelMatrix();        
        uint32_t subMeshOffset = getSubMeshOffset();        
        uint32_t materialCount = m_materialCount;
        for (uint32_t i = 0; i < subMeshCount; ++i)
        {
            _checkPreDepthMaterialValid(i);
            
            auto pMaterial = m_pPreDepthMaterials[i];
            uint32_t subMeshIndex = subMeshOffset + i;
            Material::BindInfo infoForVisualizer = {
                info.framebufferWidth,
                info.framebufferHeight,
                info.pProjMatrix,
                info.pViewMatrix,
                m_id,
                &modelMatrix,
                m_pMesh,
                subMeshIndex,
                info.hasClipRect,
                info.hasClipRect ? *(info.clipRects.data() + i) : fd::Rect2D(),
                };
    
            Material::BindResult resultForVisualizer;

            if (result.pBranchCmdBuffer != nullptr)
                resultForVisualizer.pBranchCmdBuffer = result.pBranchCmdBuffer;
            if (result.pTrunkRenderPassCmdBuffer != nullptr)
                resultForVisualizer.pTrunkRenderPassCmdBuffer = result.pTrunkRenderPassCmdBuffer;
            if (result.pTrunkWaitBarrierCmdBuffer != nullptr)
                resultForVisualizer.pTrunkWaitBarrierCmdBuffer = result.pTrunkWaitBarrierCmdBuffer;
            pMaterial->beginBind(infoForVisualizer, &resultForVisualizer);
        }
        
    }

    void BaseVisualObject::endBindForPreDepth() const
    {
        uint32_t subMeshCount = getSubMeshCount();
        uint32_t subMeshOffset = getSubMeshOffset();
        uint32_t materialCount = m_materialCount;    
        for (uint32_t i = 0; i < subMeshCount; ++i)
        {
            _checkPreDepthMaterialValid(i);
            auto pMaterial = m_pPreDepthMaterials[i];
            uint32_t subMeshIndex = subMeshOffset + i;
            Material::EndBindInfo info = {
                m_id,
                subMeshIndex,
            };
            pMaterial->endBind(info);
        }
    }

    void BaseVisualObject::beginBind(const BindInfo info, BindResult *pResult) const
    {
        auto &result = *pResult;
        uint32_t subMeshCount = getSubMeshCount();
        auto modelMatrix = _getModelMatrix();        
        uint32_t subMeshOffset = getSubMeshOffset();        
        uint32_t materialCount = m_materialCount;
        for (uint32_t i = 0; i < subMeshCount; ++i)
        {
            _checkMaterialValid(i);
            
            auto pMaterial = m_pMaterials[i];
            uint32_t subMeshIndex = subMeshOffset + i;
            Material::BindInfo infoForVisualizer = {
                info.framebufferWidth,
                info.framebufferHeight,
                info.pProjMatrix,
                info.pViewMatrix,
                m_id,
                &modelMatrix,
                m_pMesh,
                subMeshIndex,
                info.hasClipRect,
                info.hasClipRect ? *(info.clipRects.data() + i) : fd::Rect2D(),
                };
    
            Material::BindResult resultForVisualizer;

            if (result.pBranchCmdBuffer != nullptr)
                resultForVisualizer.pBranchCmdBuffer = result.pBranchCmdBuffer;
            if (result.pTrunkRenderPassCmdBuffer != nullptr)
                resultForVisualizer.pTrunkRenderPassCmdBuffer = result.pTrunkRenderPassCmdBuffer;
            if (result.pTrunkWaitBarrierCmdBuffer != nullptr)
                resultForVisualizer.pTrunkWaitBarrierCmdBuffer = result.pTrunkWaitBarrierCmdBuffer;
            pMaterial->beginBind(infoForVisualizer, &resultForVisualizer);
        }
        
    }

    void BaseVisualObject::endBind() const
    {
        uint32_t subMeshCount = getSubMeshCount();
        uint32_t subMeshOffset = getSubMeshOffset();
        uint32_t materialCount = m_materialCount;    
        for (uint32_t i = 0; i < subMeshCount; ++i)
        {
            _checkMaterialValid(i);
            auto pMaterial = m_pMaterials[i];
            uint32_t subMeshIndex = subMeshOffset + i;
            Material::EndBindInfo info = {
                m_id,
                subMeshIndex,
            };
            pMaterial->endBind(info);
        }
    }

    void BaseVisualObject::beginBindForLighting(const std::type_info &lightTypeInfo, const BindInfo info, BindResult *pResult) const
    {
        auto &result = *pResult;
        uint32_t subMeshCount = getSubMeshCount();
        auto modelMatrix = _getModelMatrix();        
        uint32_t subMeshOffset = getSubMeshOffset();        
        uint32_t materialCount = m_materialCount;

        _checkLightingMaterialValid(lightTypeInfo, 0);
        const auto &iterator = m_mapPLightingMaterials.find(std::type_index(lightTypeInfo));

        auto &pMaterials = iterator->second;

        for (uint32_t i = 0; i < subMeshCount; ++i)
        {
            _checkLightingMaterialValid(lightTypeInfo, i);
            
            auto pMaterial = pMaterials[i];
            uint32_t subMeshIndex = subMeshOffset + i;
            Material::BindInfo infoForVisualizer = {
                info.framebufferWidth,
                info.framebufferHeight,
                info.pProjMatrix,
                info.pViewMatrix,
                m_id,
                &modelMatrix,
                m_pMesh,
                subMeshIndex,
                info.hasClipRect,
                info.hasClipRect ? *(info.clipRects.data() + i) : fd::Rect2D(),
                };
    
            Material::BindResult resultForVisualizer;

            if (result.pBranchCmdBuffer != nullptr)
                resultForVisualizer.pBranchCmdBuffer = result.pBranchCmdBuffer;
            if (result.pTrunkRenderPassCmdBuffer != nullptr)
                resultForVisualizer.pTrunkRenderPassCmdBuffer = result.pTrunkRenderPassCmdBuffer;
            if (result.pTrunkWaitBarrierCmdBuffer != nullptr)
                resultForVisualizer.pTrunkWaitBarrierCmdBuffer = result.pTrunkWaitBarrierCmdBuffer;
            pMaterial->beginBind(infoForVisualizer, &resultForVisualizer);
        }
    }
        
    void BaseVisualObject::endBindForLighting(const std::type_info &lightTypeInfo) const
    {
        uint32_t subMeshCount = getSubMeshCount();
        uint32_t subMeshOffset = getSubMeshOffset();
        uint32_t materialCount = m_materialCount;    

        _checkLightingMaterialValid(lightTypeInfo, 0);
        const auto &iterator = m_mapPLightingMaterials.find(std::type_index(lightTypeInfo));

        auto &pMaterials = iterator->second;

        for (uint32_t i = 0; i < subMeshCount; ++i)
        {
            _checkLightingMaterialValid(lightTypeInfo, i);
            auto pMaterial = pMaterials[i];
            uint32_t subMeshIndex = subMeshOffset + i;
            Material::EndBindInfo info = {
                m_id,
                subMeshIndex,
            };
            pMaterial->endBind(info);
        }
    }

    // void BaseVisualObject::_asyncMeshData()
    // {
    //     if (m_subMeshCount < 0)
    //     {
    //         if (m_clipRects.size() < dynamic_cast<const ContentMesh *>(m_pMesh)->getSubMeshCount())
    //         {
    //             m_clipRects.resize(dynamic_cast<const ContentMesh *>(m_pMesh)->getSubMeshCount());
    //         }
    //     }
    // }

    void BaseVisualObject::_resizeLightingMaterialMap()
    {
        for (auto &item : m_mapPLightingMaterials)
        {
            item.second.resize(static_cast<size_t>(m_materialCount));
        }
    }

    void BaseVisualObject::_checkPreDepthMaterialValid(uint32_t index) const
    {
        auto &pMaterials = m_pPreDepthMaterials;
        if (static_cast<uint32_t>(pMaterials.size()) <= index || pMaterials[index] == nullptr)
        {
            throw std::runtime_error("The pre depth material at index " + std::to_string(index) + " is invalid.");
        }
    }
    
    void BaseVisualObject::_checkLightingMaterialValid(const std::type_info &lightTypeInfo, uint32_t index) const
    {
        size_t count = m_mapPLightingMaterials.count(std::type_index(lightTypeInfo));
        if (count == 0)
        {
            throw std::runtime_error("The lighting material is invalid.");
            return;
        }
        const auto &iterator = m_mapPLightingMaterials.find(std::type_index(lightTypeInfo));
        auto &pMaterials = iterator->second;
        if (static_cast<uint32_t>(pMaterials.size()) <= index || pMaterials[index] == nullptr)
        {
            throw std::runtime_error("The lighting material at index " + std::to_string(index) + " is invalid.");
        }
    }

    void BaseVisualObject::_checkMaterialValid(uint32_t index) const
    {
        auto &pMaterials = m_pMaterials;
        if (static_cast<uint32_t>(pMaterials.size()) <= index || pMaterials[index] == nullptr)
        {
            throw std::runtime_error("The material at index " + std::to_string(index) + " is invalid.");
        }
    }

} //namespace kgs