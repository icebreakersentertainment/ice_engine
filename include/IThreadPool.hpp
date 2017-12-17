#ifndef ITHREADPOOL_H_
#define ITHREADPOOL_H_

#include <functional>
#include <future>

#include "Types.hpp"

namespace ice_engine
{

class IThreadPool
{
public:
	virtual ~IThreadPool()
	{
	}
	;
	
	virtual std::future<void> postWork(const std::function<void()>& work) = 0;
	virtual std::future<void> postWork(std::function<void()>&& work) = 0;
	virtual void waitAll() = 0;
	virtual void joinAll() = 0;
	
	virtual uint32 getActiveWorkerCount() const = 0;
	virtual uint32 getInactiveWorkerCount() const = 0;
	
	virtual uint32 getWorkQueueCount() const = 0;
	virtual uint32 getWorkQueueSize() const = 0;
	virtual void increaseWorkerCountBy(uint32 n) = 0;
	virtual void decreaseWorkerCountBy(uint32 n) = 0;
};

}

#endif /* ITHREADPOOL_H_ */
