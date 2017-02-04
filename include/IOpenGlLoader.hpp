#ifndef IOPENGLLOADER_H_
#define IOPENGLLOADER_H_

namespace hercules
{

class IOpenGlLoader
{
public:
	virtual ~IOpenGlLoader()
	{
	}
	;
	
	virtual void postWork(const std::function<void()>& work) = 0;
	virtual void waitAll() = 0;
	
	virtual unsigned int getWorkQueueCount() const = 0;
	
	virtual void tick() = 0;
	
	virtual void block() = 0;
	virtual void unblock() = 0;
};

}

#endif /* IOPENGLLOADER_H_ */
