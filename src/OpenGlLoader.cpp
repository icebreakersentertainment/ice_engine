#include "OpenGlLoader.hpp"

namespace ice_engine
{

OpenGlLoader::OpenGlLoader()
{
	initialize();
}

OpenGlLoader::~OpenGlLoader()
{
}

void OpenGlLoader::initialize()
{
}

std::future<void> OpenGlLoader::postWork(const std::function<void()>& work)
{
	auto task = std::make_shared<std::packaged_task<void()>>([work = work]() {
		work();
	});
	
	std::lock_guard<std::mutex> lockGuard(enqueuedWorkMutex_);
	enqueuedWork_.push_back(task);
	
	return task->get_future();
}

std::future<void> OpenGlLoader::postWork(std::function<void()>&& work)
{
	auto task = std::make_shared<std::packaged_task<void()>>([work = std::move(work)]() {
		work();
	});
	
	std::lock_guard<std::mutex> lockGuard(enqueuedWorkMutex_);
	enqueuedWork_.push_back(task);
	
	return task->get_future();
}

void OpenGlLoader::waitAll()
{
	// TODO: implement
}

uint32 OpenGlLoader::getWorkQueueCount() const
{
	uint size = 0;
	
	std::lock_guard<std::mutex> lockGuard(enqueuedWorkMutex_);
	size = enqueuedWork_.size();
	
	return size;
}

void OpenGlLoader::block()
{
	enqueuedWorkMutex_.lock();
}

void OpenGlLoader::unblock()
{
	enqueuedWorkMutex_.unlock();
}

void OpenGlLoader::tick()
{
	std::lock_guard<std::mutex> lockGuard(enqueuedWorkMutex_);
	if (enqueuedWork_.size() > 0)
	{
		auto work = enqueuedWork_.front();
		enqueuedWork_.pop_front();
		
		(*work)();
	}
}

}
