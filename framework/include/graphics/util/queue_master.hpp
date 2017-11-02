#ifndef KGS_QUEUE_MASTER_H
#define KGS_QUEUE_MASTER_H

#include <queue>
#include <unordered_map>
#include "graphics/global.hpp"

namespace kgs
{
	class QueueMaster
	{
	public:
		QueueMaster(std::shared_ptr<vk::Device> pDevice
			, uint32_t graphicsFamily
			, uint32_t graphicsQueueCount
		    , uint32_t presentFamily
	        , uint32_t presentQueueCount
		);

		uint32_t getQueueCount(uint32_t family);
		void allocateQueue(uint32_t family, uint32_t &queueIndex, vk::Queue &queue);
		void freeQueue(uint32_t family, uint32_t queueIndex);

	private:
		QueueMaster() = delete;
		std::shared_ptr<vk::Device> m_pDevice;
		uint32_t m_graphicsFamily;
		uint32_t m_graphicsQueueCount;
		uint32_t m_presentFamily;
		uint32_t m_presentQueueCount;
		std::unordered_map<uint32_t, std::queue<uint32_t>> m_mapQueueIndexs;

		void _createQueueIndexs();
	};
} //namespace kgs

#endif // !KGS_QUEUE_MASTER_H
