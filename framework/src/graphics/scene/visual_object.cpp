#include "graphics/scene/visual_object.hpp"

namespace kgs
{
	std::shared_ptr<Material> BaseVisualObject::getMaterial()
	{
		return m_pMaterial;
	}

	void BaseVisualObject::setPMaterial(std::shared_ptr<Material> pMaterial)
	{
		m_pMaterial = pMaterial;
	}
} //namespace kgs