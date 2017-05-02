#ifndef MEMORY_POOL_H_
#define MEMORY_POOL_H_

#include <memory>

#include "boost/pool/pool.hpp"

#include "Types.hpp"

namespace hercules
{
namespace memory
{

template <typename T>
class MemoryPool
{
public:
	MemoryPool()
	{
		pool_ = std::make_unique<boost::pool<>>(sizeof(T));
	};
	
	virtual ~MemoryPool()
	{
	};
	
	T* malloc()
	{
		return reinterpret_cast<T*>(pool_->ordered_malloc());
	}
	
	void free(T* chunk)
	{
		pool_->free(chunk);
	}
	
	bool isFrom(T* chunk)
	{
		return pool_->is_from(chunk);
	}
	
	bool releaseMemory()
	{
		return pool_->release_memory();
	}
	
	bool purgeMemory()
	{
		return pool_->purge_memory();
	}
	
private:
	std::unique_ptr<boost::pool<>> pool_;
};

}
}

#endif /* MEMORY_POOL_H_ */
