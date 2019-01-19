#include "graphics/scene/visual_object_2.hpp"

#include "graphics/util/gemo_util.hpp"

namespace vg
{
    VisualObject2::VisualObject2()
        : VisualObject<SpaceType::SPACE_2>()
    {

    }

    Bool32 VisualObject2::getHasClipRect() const
    {
        return m_hasClipRect;
    }

    void VisualObject2::setHasClipRect(Bool32 value)
    {
        m_hasClipRect = value;
    }

    uint32_t VisualObject2::getClipRectCount() const
    {
        return static_cast<uint32_t>(m_clipRects.size());
    }
        
    const fd::Rect2D *VisualObject2::getClipRects() const
    {
        return m_clipRects.data();
    }

    void VisualObject2::updateClipRects(fd::ArrayProxy<fd::Rect2D> rects, uint32_t offset)
    {
        uint32_t count = static_cast<uint32_t>(rects.size());
        if (m_clipRects.size() < offset + count) m_clipRects.resize(offset + count);
        for (uint32_t i = 0; i < count; ++i)
        {
            m_clipRects[offset] = *(rects.data() + i);
            ++offset;
        }
    }
        
    void VisualObject2::updateClipRects(fd::Rect2D rect, uint32_t count, uint32_t offset)
    {
        if (m_clipRects.size() < offset + count) m_clipRects.resize(offset + count);
        for (uint32_t i = 0; i < count; ++i)
        {
            m_clipRects[offset] = rect;
            ++offset;
        }
    }

    Matrix4x4 VisualObject2::_getModelMatrix() const
    {
        return tranMat3ToMat4(m_pTransform->getMatrixLocalToWorld());
    }

    void VisualObject2::setMesh(MeshDimType *pMesh)
    {
        VisualObject<SpaceType::SPACE_2>::setMesh(pMesh);
        m_clipRects.resize(dynamic_cast<const ContentMesh *>(m_pMesh)->getSubMeshCount());
    }

    void VisualObject2::setMesh(MeshDimType *pMesh
       , uint32_t subMeshOffset
       , uint32_t subMeshCount)
    {
        VisualObject<SpaceType::SPACE_2>::setMesh(pMesh, subMeshOffset, subMeshCount);
        m_clipRects.resize(subMeshCount);
    }

    void VisualObject2::updateSubMeshInfo(uint32_t subMeshOffset, uint32_t subMeshCount)
    {
        VisualObject<SpaceType::SPACE_2>::updateSubMeshInfo(subMeshOffset, subMeshCount);
        m_clipRects.resize(subMeshCount);
    }

} //namespace kgs