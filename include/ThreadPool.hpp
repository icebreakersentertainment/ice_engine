#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include "threadpool11/threadpool11.h"

#include "IThreadPool.hpp"

namespace game
{

class ThreadPool : public IThreadPool
{
public:
	ThreadPool();
	ThreadPool(int numThreads);
	virtual ~ThreadPool();
	
	virtual void postWork(const std::function<void()>& work);
	virtual void waitAll();
	virtual void joinAll();
	
	virtual unsigned int getActiveWorkerCount();
	virtual unsigned int getInactiveWorkerCount();
	
	virtual unsigned int getWorkQueueCount();
	virtual void increaseWorkerCountBy(unsigned int n);
	virtual void decreaseWorkerCountBy(unsigned int n);
	
private:
	std::unique_ptr<threadpool11::Pool> pool_;
	
	void initialize(int numThreads);
};

}

#endif /* THREADPOOL_H_ */
