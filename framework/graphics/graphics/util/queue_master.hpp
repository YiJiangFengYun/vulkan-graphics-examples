#ifndef VG_QUEUE_MASTER_H
#define VG_QUEUE_MASTER_H

#include <queue>
#include <unordered_map>
#include "graphics/global.hpp"

namespace vg
{
	class QueueMaster
	{
	public:
		QueueMaster(std::shared_ptr<vk::Device> pDevice
			, std::vector<std::pair<uint32_t, uint32_t>> familiesAndCounts
		);

		QueueMaster(std::shared_ptr<vk::Device> pDevice
			, std::unordered_map<uint32_t, uint32_t> familiesAndCounts
		);

		uint32_t getQueueCount(uint32_t family);
		void allocateQueue(uint32_t family, uint32_t &queueIndex, vk::Queue &queue);
		void freeQueue(uint32_t family, uint32_t queueIndex);

	private:
		QueueMaster() = delete;
		std::shared_ptr<vk::Device> m_pDevice;
		std::unordered_map<uint32_t, std::queue<uint32_t>> m_mapQueueIndexs;

		void _createQueueIndexs(std::vector<std::pair<uint32_t, uint32_t>> familiesAndCounts);
		void _createQueueIndexs(std::unordered_map<uint32_t, uint32_t> familiesAndCounts);
	};
} //namespace kgs

#endif // !VG_QUEUE_MASTER_H
