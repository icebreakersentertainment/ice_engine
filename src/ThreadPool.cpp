#include "ThreadPool.hpp"

namespace hercules
{

ThreadPool::ThreadPool()
{
	initialize( std::thread::hardware_concurrency() );
}

ThreadPool::ThreadPool(uint32 numThreads)
{
	initialize( numThreads );
}

ThreadPool::~ThreadPool()
{
	this->joinAll();
}

void ThreadPool::initialize(uint32 numThreads)
{
	pool_ = std::unique_ptr<ctpl::thread_pool>( new ctpl::thread_pool((size_t)numThreads) );
}

void ThreadPool::postWork(const std::function<void()>& work)
{
	pool_->push( [&work] (int32 id) { work(); } );
}

void ThreadPool::waitAll()
{
	pool_->stop(false);
}

void ThreadPool::joinAll()
{
	pool_->stop(true);
}

uint32 ThreadPool::getWorkQueueCount()
{
	return pool_->size();
}

uint32 ThreadPool::getActiveWorkerCount()
{
	return (this->getWorkQueueCount() - this->getInactiveWorkerCount());
}

uint32 ThreadPool::getInactiveWorkerCount()
{
	return pool_->n_idle();
}

void ThreadPool::increaseWorkerCountBy(uint32 n)
{
	pool_->resize((size_t)n);
}

void ThreadPool::decreaseWorkerCountBy(uint32 n)
{
	pool_->resize((size_t)n);
}

}
