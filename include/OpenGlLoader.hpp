#ifndef OPENGLLOADER_H_
#define OPENGLLOADER_H_

#include <mutex>
#include <deque>
#include <memory>

#include "IOpenGlLoader.hpp"

namespace ice_engine
{

class OpenGlLoader : public IOpenGlLoader
{
public:
	OpenGlLoader();
	virtual ~OpenGlLoader();
	
	virtual std::future<void> postWork(const std::function<void()>& work) override;
	virtual std::future<void> postWork(std::function<void()>&& work) override;
	virtual void waitAll() override;
	
	virtual uint32 getWorkQueueCount() const override;
	
	virtual void tick() override;
	
	virtual void block() override;
	virtual void unblock() override;
	
private:
	mutable std::mutex enqueuedWorkMutex_;
	std::deque< std::shared_ptr<std::packaged_task<void()>> > enqueuedWork_;

	void initialize();
};

}

#endif /* OPENGLLOADER_H_ */
