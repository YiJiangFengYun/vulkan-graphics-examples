#include "framework/util/thread_master.hpp"

namespace vgf
{
	ThreadMaster::ThreadMaster()
	{
	}

	ThreadMaster::~ThreadMaster()
	{
	}

	void ThreadMaster::appendThread(const std::shared_ptr<std::thread> pThread)
	{
		m_pThreads.push_back(pThread);
	}

	void ThreadMaster::join()
	{
		for (const auto& pthread : m_pThreads)
		{
			if (pthread->joinable())
			{
				pthread->join();
			}
		}
	}
}