#ifndef OPENGLLOADER_H_
#define OPENGLLOADER_H_

#include <mutex>
#include <deque>

#include "IOpenGlLoader.hpp"

namespace game
{

class OpenGlLoader : public IOpenGlLoader
{
public:
	OpenGlLoader();
	virtual ~OpenGlLoader();
	
	virtual void postWork(const std::function<void()>& work);
	virtual void waitAll();
	
	virtual unsigned int getWorkQueueCount() const;
	
	virtual void tick();
	
	virtual void block();
	virtual void unblock();
	
private:
	mutable std::mutex enqueuedWorkMutex_;
	std::deque< std::function<void()> > enqueuedWork_;

	void initialize();
};

}

#endif /* OPENGLLOADER_H_ */
