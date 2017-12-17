#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <ctpl/ctpl.h>

#include "IThreadPool.hpp"

namespace ice_engine
{

class ThreadPool : public IThreadPool
{
public:
	ThreadPool();
	ThreadPool(uint32 numThreads);
	virtual ~ThreadPool();
	
	virtual std::future<void> postWork(const std::function<void()>& work) override;
	virtual std::future<void> postWork(std::function<void()>&& work) override;
	virtual void waitAll() override;
	virtual void joinAll() override;
	
	virtual uint32 getActiveWorkerCount() const override;
	virtual uint32 getInactiveWorkerCount() const override;
	
	virtual uint32 getWorkQueueCount() const override;
	virtual uint32 getWorkQueueSize() const override;
	virtual void increaseWorkerCountBy(uint32 n) override;
	virtual void decreaseWorkerCountBy(uint32 n) override;
	
	//void postWork(const std::function<void()>& work) override;
	
private:
	std::unique_ptr<ctpl::thread_pool> pool_;
	
	void initialize(uint32 numThreads);
};

}

#endif /* THREADPOOL_H_ */
