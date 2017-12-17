#ifndef IOPENGLLOADER_H_
#define IOPENGLLOADER_H_

#include <functional>
#include <future>

#include "Types.hpp"

namespace ice_engine
{

class IOpenGlLoader
{
public:
	virtual ~IOpenGlLoader()
	{
	}
	;
	
	virtual std::future<void> postWork(const std::function<void()>& work) = 0;
	virtual std::future<void> postWork(std::function<void()>&& work) = 0;
	virtual void waitAll() = 0;
	
	virtual uint32 getWorkQueueCount() const = 0;
	
	virtual void tick() = 0;
	
	virtual void block() = 0;
	virtual void unblock() = 0;
};

}

#endif /* IOPENGLLOADER_H_ */
