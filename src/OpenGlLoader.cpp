#include "OpenGlLoader.hpp"
#include <thread>

//#include <GL/glx.h>

namespace hercules
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

void OpenGlLoader::postWork(const std::function<void()>& work)
{
	enqueuedWorkMutex_.lock();
	enqueuedWork_.emplace_back( std::move(work) );
	enqueuedWorkMutex_.unlock();
}

void OpenGlLoader::waitAll()
{
	// TODO: implement
}

unsigned int OpenGlLoader::getWorkQueueCount() const
{
	auto size = 0;
	
	enqueuedWorkMutex_.lock();
	size = enqueuedWork_.size();
	enqueuedWorkMutex_.unlock();
	
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
	bool doWork = false;
	
	enqueuedWorkMutex_.lock();
	if (enqueuedWork_.size() > 0)
		doWork = true;
	enqueuedWorkMutex_.unlock();
	
	if (doWork)
	{
		std::function<void()> work;
		enqueuedWorkMutex_.lock();
		work = std::move(enqueuedWork_.front());
		enqueuedWork_.pop_front();
		enqueuedWorkMutex_.unlock();
		
		work();
	}
}

}
