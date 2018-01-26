#include "graphics/scene/visual_object.hpp"

namespace vg
{
	BaseVisualObject::BaseVisualObject()
	{

	}

	std::shared_ptr<Material> BaseVisualObject::getMaterial()
	{
		return m_pMaterial;
	}

	void BaseVisualObject::setMaterial(std::shared_ptr<Material> pMaterial)
	{
		m_pMaterial = pMaterial;
	}

	std::shared_ptr<SepMesh> BaseVisualObject::getMesh()
	{
		return m_pMesh;
	}
} //namespace kgs