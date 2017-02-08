#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <ctpl/ctpl.h>

#include "IThreadPool.hpp"

namespace hercules
{

class ThreadPool : public IThreadPool
{
public:
	ThreadPool();
	ThreadPool(uint32 numThreads);
	virtual ~ThreadPool();
	
	virtual void postWork(const std::function<void()>& work) override;
	virtual void waitAll() override;
	virtual void joinAll() override;
	
	virtual uint32 getActiveWorkerCount() override;
	virtual uint32 getInactiveWorkerCount() override;
	
	virtual uint32 getWorkQueueCount() override;
	virtual void increaseWorkerCountBy(uint32 n) override;
	virtual void decreaseWorkerCountBy(uint32 n) override;
	
private:
	std::unique_ptr<ctpl::thread_pool> pool_;
	
	void initialize(uint32 numThreads);
};

}

#endif /* THREADPOOL_H_ */
