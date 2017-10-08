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
		auto iterator2 = std::find(m_arrPasses.cbegin(), m_arrPasses.cend(), iterator->second);
		return static_cast<int32_t>(iterator2 - m_arrPasses.cbegin());

	}

	std::string Material::getPassName(int32_t index)
	{
		auto pass = *(m_arrPasses.cbegin() + index);
		//linear complexity
		auto pos = std::find_if(m_mapPasses.cbegin(), m_mapPasses.cend(), [pass](const std::pair<std::string, std::shared_ptr<Pass>>& elem) {
			return elem.second == pass;
		});
		return pos->first;
	}

	std::shared_ptr<Pass> Material::getPass(std::string name)
	{
		return getValue(name, m_mapPasses, m_arrPasses);
	}

	void Material::setPass(std::string name, std::shared_ptr<Pass> pass)
	{
		setValue(name, pass, m_mapPasses, m_arrPasses);
	}

	void Material::apply()
	{
		for (const auto& item : m_arrPasses)
		{
			item->apply();
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