#include "graphics/material/material.hpp"

namespace vg
{
	Material::Material()
		: Base(BaseType::MATERIAL)
		, m_renderQueueType()
		, m_renderPriority()
		, m_pVisualizers()
		, m_unusedVisualizerCount()
		, m_pUnusedVisualizes()
		, m_arrPasses()
		, m_mapPasses()
	{
	}

	Material::~Material()
	{
	}

	uint32_t Material::getPassCount() const
	{
		return static_cast<uint32_t>(m_arrPasses.size());
	}

	Pass *Material::getPassWithIndex(uint32_t index) const
	{
		return m_arrPasses[index];
	}

	Pass * const *Material::getPasses() const
	{
		return m_arrPasses.data();
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

	Visualizer *Material::allocateVisualizer(Visualizer **ppVisualizer)
	{
		if (m_unusedVisualizerCount > 0)
		{
			Visualizer *pVisualizer = m_pUnusedVisualizes[m_unusedVisualizerCount - 1];
			--m_unusedVisualizerCount;
			if(ppVisualizer != nullptr)(*ppVisualizer) = pVisualizer;
			return pVisualizer;
		}
		else
		{
			Visualizer *pVisualizer = _createVisualizer();
			m_pVisualizers.push_back(std::shared_ptr<Visualizer>{pVisualizer});
			if(ppVisualizer != nullptr)(*ppVisualizer) = pVisualizer;
			return pVisualizer;
		}
	}

	void Material::deallocateVisualizer(Visualizer *pVisualizer)
	{
		m_pUnusedVisualizes[m_unusedVisualizerCount] = pVisualizer;
		++m_unusedVisualizerCount;
	}
		
	void Material::clearVisualizers()
	{
		m_pVisualizers.resize(0u);
		m_pUnusedVisualizes.resize(0u);
		m_unusedVisualizerCount = 0u;
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

	Visualizer *Material::_createVisualizer()
	{
		return new Visualizer();
	}
}