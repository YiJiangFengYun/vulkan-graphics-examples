#include "graphics/util/queue_master.hpp"

namespace kgs
{
	QueueMaster::QueueMaster(std::shared_ptr<vk::Device> pDevice
		, uint32_t graphicsFamily
		, uint32_t graphicsQueueCount
		, uint32_t presentFamily
		, uint32_t presentQueueCount
	)
		: m_pDevice(pDevice)
		, m_graphicsFamily(graphicsFamily)
		, m_graphicsQueueCount(graphicsQueueCount)
		, m_presentFamily(presentFamily)
		, m_presentQueueCount(presentQueueCount)
	{
		_createQueueIndexs();
	}

	uint32_t  QueueMaster::getQueueCount(uint32_t family)
	{
		return static_cast<uint32_t>(m_mapQueueIndexs[family].size());
	}
	void QueueMaster::allocateQueue(uint32_t family, uint32_t &queueIndex, vk::Queue &queue)
	{
		if (m_mapQueueIndexs[family].size() == 0)
			throw std::runtime_error("Queue count is zero, its queue family: " + family);

		queueIndex = m_mapQueueIndexs[family].front();
		m_mapQueueIndexs[family].pop();
		queue = m_pDevice->getQueue(family, queueIndex);
	}

	void QueueMaster::freeQueue(uint32_t family, uint32_t queueIndex)
	{
		m_mapQueueIndexs[family].push(queueIndex);
	}

	void QueueMaster::_createQueueIndexs()
	{
		std::unordered_map<uint32_t, uint32_t> mapCounts;
		mapCounts[m_graphicsFamily] += m_graphicsQueueCount;
		mapCounts[m_presentFamily] += m_presentQueueCount;

		for (const auto& item : mapCounts)
		{
			uint32_t count = item.second;
			for (uint32_t i = 0; i < count; ++i)
			{
				m_mapQueueIndexs[item.first].push(i);
			}
		}
	}
} //namespace kgs