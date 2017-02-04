#include "ThreadPool.hpp"

namespace hercules
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
	this->joinAll();
}

void ThreadPool::initialize(int numThreads)
{
	pool_ = std::unique_ptr<ctpl::thread_pool>( new ctpl::thread_pool((size_t)numThreads) );
}

void ThreadPool::postWork(const std::function<void()>& work)
{
	pool_->push( [&work] (int id) { work(); } );
}

void ThreadPool::waitAll()
{
	pool_->stop(false);
}

void ThreadPool::joinAll()
{
	pool_->stop(true);
}

unsigned int ThreadPool::getWorkQueueCount()
{
	return pool_->size();
}

unsigned int ThreadPool::getActiveWorkerCount()
{
	return (this->getWorkQueueCount() - this->getInactiveWorkerCount());
}

unsigned int ThreadPool::getInactiveWorkerCount()
{
	return pool_->n_idle();
}

void ThreadPool::increaseWorkerCountBy(unsigned int n)
{
	pool_->resize((size_t)n);
}

void ThreadPool::decreaseWorkerCountBy(unsigned int n)
{
	pool_->resize((size_t)n);
}

}
