#ifndef ENTITIES_RESOURCE_HANDLE_H_
#define ENTITIES_RESOURCE_HANDLE_H_

#include "Types.hpp"

namespace hercules
{
namespace entities
{

class ResourceHandle
{
public:
	ResourceHandle() : id_(0)
	{
	}

	explicit ResourceHandle(uint64 id) : id_(id)
	{
	}

	virtual ~ResourceHandle()
	{
	}
	
	uint64 getId() const
	{
		return id_;
	} 
	
	bool operator == (const ResourceHandle& other) const
	{
		return id_ == other.id_;
	}

	bool operator != (const ResourceHandle& other) const
	{
		return id_ != other.id_;
	}

	bool operator < (const ResourceHandle& other) const
	{
		return id_ < other.id_;
	}

	bool operator > (const ResourceHandle& other) const
	{
		return other.id_ < id_;
	}

	bool operator <= (const ResourceHandle& other) const
	{
		return !(id_ > other.id_);
	}

	bool operator >= (const ResourceHandle& other) const
	{
		return !(id_ < other.id_);
	}

	static const ResourceHandle INVALID;
private:
	uint64 id_;
};

}
}

#endif /* ENTITIES_RESOURCE_HANDLE_H_ */
