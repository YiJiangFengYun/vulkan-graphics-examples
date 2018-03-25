#include "graphics/scene/visual_object.hpp"

namespace vg
{
	BaseVisualObject::BaseVisualObject()
	    : m_pMaterial()
		, m_pMesh()
		, m_subMeshOffset(-1)
		, m_subMeshCount(-1)
		, m_isVisibilityCheck(VG_TRUE)
		, m_hasClipRect(VG_FALSE)
		, m_clipRects()
	{

	}

	BaseVisualObject::~BaseVisualObject()
	{

	}

	Material *BaseVisualObject::getMaterial() const
	{
		return m_pMaterial;
	}

	void BaseVisualObject::setMaterial(Material *pMaterial)
	{
		m_pMaterial = pMaterial;
	}

	BaseMesh *BaseVisualObject::getMesh() const
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
		m_clipRects.resize(subMeshCount);
	}

	Bool32 BaseVisualObject::getIsVisibilityCheck() const
	{
		return m_isVisibilityCheck;
	}

	void BaseVisualObject::setIsVisibilityCheck(Bool32 value)
	{
		m_isVisibilityCheck = value;
	}

	Bool32 BaseVisualObject::getHasClipRect() const
	{
		return m_hasClipRect;
	}

	void BaseVisualObject::setHasClipRect(Bool32 value)
	{
		m_hasClipRect = value;
	}

	uint32_t BaseVisualObject::getClipRectCount() const
	{
		return static_cast<uint32_t>(m_clipRects.size());
	}
		
	const fd::Rect2D *BaseVisualObject::getClipRects() const
	{
		return m_clipRects.data();
	}

	void BaseVisualObject::updateClipRects(fd::ArrayProxy<fd::Rect2D> rects, uint32_t count, uint32_t offset)
	{
		_asyncMeshData();
		for (uint32_t i = 0; i < count; ++i)
        {
            m_clipRects[offset] = *(rects.data() + i);
            ++offset;
        }
	}
		
	void BaseVisualObject::updateClipRects(fd::Rect2D rect, uint32_t count, uint32_t offset)
	{
		_asyncMeshData();
		for (uint32_t i = 0; i < count; ++i)
        {
			m_clipRects[offset] = rect;
            ++offset;
        }
	}


	void BaseVisualObject::_asyncMeshData()
	{
		if (m_subMeshCount < 0)
		{
			if (m_clipRects.size() < dynamic_cast<const ContentMesh *>(m_pMesh)->getSubMeshCount())
			{
				m_clipRects.resize(dynamic_cast<const ContentMesh *>(m_pMesh)->getSubMeshCount());
			}
		}
	}

} //namespace kgs