#ifndef GFW_THREAD_MASTER_H
#define GFW_THREAD_MASTER_H

#include <vector>
#include <memory>
#include <thread>

namespace gfw
{
	class ThreadMaster
	{
	public:
		ThreadMaster();
		~ThreadMaster();
		void appendThread(const std::shared_ptr<std::thread> pThread);

		void join();
	private:
		std::vector<std::shared_ptr<std::thread>> m_pThreads;
	};
}

#endif // !GFW_THREAD_MASTER_H
