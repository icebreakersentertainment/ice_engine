#ifndef ITHREADPOOL_H_
#define ITHREADPOOL_H_

#include <functional>

namespace game
{

class IThreadPool
{
public:
	virtual ~IThreadPool()
	{
	}
	;
	
	virtual void postWork(const std::function<void()>& work) = 0;
	virtual void waitAll() = 0;
	virtual void joinAll() = 0;
	
	virtual unsigned int getActiveWorkerCount() = 0;
	virtual unsigned int getInactiveWorkerCount() = 0;
	
	virtual unsigned int getWorkQueueCount() = 0;
	virtual void increaseWorkerCountBy(unsigned int n) = 0;
	virtual void decreaseWorkerCountBy(unsigned int n) = 0;
};

}

#endif /* ITHREADPOOL_H_ */
