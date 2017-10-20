#include "graphics/material/material.hpp"

namespace kgs
{
	Material::Material()
	{
	}

	Material::~Material()
	{
	}

	int32_t Material::getPassIndex(std::string name)
	{
		auto iterator = m_mapPasses.find(name);
		if (iterator == m_mapPasses.end()) return -1;
		//linear complexity
		auto iterator2 = std::find(m_arrPasseNames.cbegin(), m_arrPasseNames.cend(), iterator->second);
		return static_cast<int32_t>(iterator2 - m_arrPasseNames.cbegin());

	}

	std::string Material::getPassName(uint32_t index)
	{
		return  m_arrPasseNames[index];
	}

	const std::shared_ptr<Pass>& Material::getPass(std::string name) const
	{
		return getValue(name, m_mapPasses);
	}

	void Material::setPass(std::string name, const std::shared_ptr<Pass> &pass)
	{
		setValue(name, pass, m_mapPasses, m_arrPasseNames);
	}

	const std::shared_ptr<Pass>& Material::getPassWithIndex(uint32_t index) const
	{
		auto name = m_arrPasseNames[index];
		return getValue(name, m_mapPasses);
	}

	uint32_t Material::getPassCount()
	{
		return static_cast<uint32_t>(m_arrPasseNames.size());
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