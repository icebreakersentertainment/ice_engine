#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <memory>

#include "IThreadPool.hpp"

namespace ctpl
{
class thread_pool;
}

namespace ice_engine
{

class ThreadPool : public IThreadPool
{
public:
	ThreadPool();
	ThreadPool(uint32 numThreads);
	~ThreadPool() override;
	
	std::future<void> postWork(const std::function<void()>& work) override;
	std::future<void> postWork(std::function<void()>&& work) override;
	void waitAll() override;
	void joinAll() override;
	
	uint32 getActiveWorkerCount() const override;
	uint32 getInactiveWorkerCount() const override;
	
	uint32 getWorkQueueCount() const override;
	uint32 getWorkQueueSize() const override;
	void increaseWorkerCountBy(uint32 n) override;
	void decreaseWorkerCountBy(uint32 n) override;
	
	//void postWork(const std::function<void()>& work) override;
	
private:
	std::unique_ptr<ctpl::thread_pool> pool_;
	
	void initialize(uint32 numThreads);
};

}

#endif /* THREADPOOL_H_ */
