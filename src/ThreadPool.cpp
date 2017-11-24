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
	pool_ = std::make_unique<ctpl::thread_pool>(static_cast<size_t>(numThreads));
}

std::future<void> ThreadPool::postWork(const std::function<void()>& work)
{
	return pool_->push( [=] (int32 id) { work(); } );
}

std::future<void> ThreadPool::postWork(std::function<void()>&& work)
{
	return pool_->push( [work = std::move(work)] (int32 id) { work(); } );
}

void ThreadPool::waitAll()
{
	pool_->stop(false);
}

void ThreadPool::joinAll()
{
	pool_->stop(true);
}

uint32 ThreadPool::getWorkQueueCount() const
{
	return 0;//pool_->n_pending();
}

uint32 ThreadPool::getWorkQueueSize() const
{
	return pool_->size();
}

uint32 ThreadPool::getActiveWorkerCount() const
{
	return (this->getWorkQueueSize() - this->getInactiveWorkerCount());
}

uint32 ThreadPool::getInactiveWorkerCount() const
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
