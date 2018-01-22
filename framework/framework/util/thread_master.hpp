#ifndef VGF_THREAD_MASTER_H
#define VGF_THREAD_MASTER_H

#include <vector>
#include <memory>
#include <thread>

namespace vgf
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

#endif // !VGF_THREAD_MASTER_H
