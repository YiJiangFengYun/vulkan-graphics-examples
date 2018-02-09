#include "graphics/scene/visual_object.hpp"

namespace vg
{
	BaseVisualObject::BaseVisualObject()
	{

	}

	const std::shared_ptr<Material> &BaseVisualObject::getMaterial() const
	{
		return m_pMaterial;
	}

	void BaseVisualObject::setMaterial(std::shared_ptr<Material> pMaterial)
	{
		m_pMaterial = pMaterial;
	}

	const std::shared_ptr<BaseMesh> &BaseVisualObject::getMesh() const
	{
		return m_pMesh;
	}

	uint32_t BaseVisualObject::getSubMeshOffset() const
	{
		return m_subMeshOffset;
	}
		
	uint32_t BaseVisualObject::getSubMeshCount() const
	{
		return m_subMeshCount;
	}

	void BaseVisualObject::updateSubMeshInfo(uint32_t subMeshOffset, uint32_t subMeshCount)
	{
		m_subMeshOffset = subMeshOffset;
		m_subMeshCount = subMeshCount;
	}

} //namespace kgs