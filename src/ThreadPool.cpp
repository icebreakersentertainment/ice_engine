#include "ThreadPool.hpp"

namespace game
{

ThreadPool::ThreadPool()
{
	initialize( std::thread::hardware_concurrency() );
}

ThreadPool::ThreadPool(int numThreads)
{
	initialize( numThreads );
}

ThreadPool::~ThreadPool()
{
	pool_->joinAll();
}

void ThreadPool::initialize(int numThreads)
{
	//pool_ = threadpool11::Pool( (size_t)numThreads );
	pool_ = std::unique_ptr<threadpool11::Pool>( new threadpool11::Pool((size_t)numThreads) );
}

void ThreadPool::postWork(const std::function<void()>& work)
{
	pool_->postWork( work );
}

void ThreadPool::waitAll()
{
	pool_->waitAll();
}

void ThreadPool::joinAll()
{
	pool_->joinAll();
}

unsigned int ThreadPool::getWorkQueueCount()
{
	return pool_->getWorkQueueCount();
}

unsigned int ThreadPool::getActiveWorkerCount()
{
	return pool_->getActiveWorkerCount();
}

unsigned int ThreadPool::getInactiveWorkerCount()
{
	return pool_->getInactiveWorkerCount();
}

void ThreadPool::increaseWorkerCountBy(unsigned int n)
{
	pool_->increaseWorkerCountBy((size_t)n);
}

void ThreadPool::decreaseWorkerCountBy(unsigned int n)
{
	pool_->decreaseWorkerCountBy((size_t)n);
}

}
