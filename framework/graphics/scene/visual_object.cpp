#include "graphics/scene/visual_object.hpp"

namespace vg
{
	BaseVisualObject::BaseVisualObject()
	    : m_pMaterial()
		, m_pMesh()
		, m_subMeshOffset(-1)
		, m_subMeshCount(-1)
		, m_isVisibilityCheck(VG_TRUE)
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
	}

	Bool32 BaseVisualObject::getIsVisibilityCheck() const
	{
		return m_isVisibilityCheck;
	}

	void BaseVisualObject::setIsVisibilityCheck(Bool32 value)
	{
		m_isVisibilityCheck = value;
	}

} //namespace kgs