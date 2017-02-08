#ifndef ITHREADPOOL_H_
#define ITHREADPOOL_H_

#include <functional>

#include "Types.hpp"

namespace hercules
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
	
	virtual uint32 getActiveWorkerCount() = 0;
	virtual uint32 getInactiveWorkerCount() = 0;
	
	virtual uint32 getWorkQueueCount() = 0;
	virtual void increaseWorkerCountBy(uint32 n) = 0;
	virtual void decreaseWorkerCountBy(uint32 n) = 0;
};

}

#endif /* ITHREADPOOL_H_ */
