#include "graphics/material/material.hpp"

namespace vg
{
	Material::Material()
		: Base(BaseType::MATERIAL)
	{
	}

	Material::~Material()
	{
	}

	uint32_t Material::getPassCount()
	{
		return static_cast<uint32_t>(m_arrPasses.size());
	}

	Pass *Material::getPassWithIndex(uint32_t index) const
	{
		return m_arrPasses[index];
	}

	Bool32 Material::isHas(const Pass *pass) const
	{
		return m_mapPasses.find(pass->getID()) != m_mapPasses.cend();
	}

	void Material::addPass(Pass *pPass)
	{
		if (isHas(pPass)) return;
		m_arrPasses.push_back(pPass);
		m_mapPasses[pPass->getID()] = pPass;
	}

	void Material::removePass(Pass *pPass)
	{
		if (isHas(pPass) == VG_FALSE) return;
		std::remove(m_arrPasses.begin(), m_arrPasses.end(), pPass);
		m_mapPasses.erase(pPass->getID());
	}

	void Material::apply()
	{
		for (const auto& item : m_mapPasses)
		{
			item.second->apply();
		}
	}

	MaterialShowType Material::getShowType()
	{
		return m_renderQueueType;
	}

	void Material::setRenderQueueType(MaterialShowType type)
	{
		m_renderQueueType = type;
	}

	uint32_t Material::getRenderPriority()
	{
		return m_renderPriority;
	}

	void Material::setRenderPriority(uint32_t priority)
	{
		m_renderPriority = priority;
	}
}