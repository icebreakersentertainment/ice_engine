#ifndef GRAPHICS_RESOURCE_HANDLE_H_
#define GRAPHICS_RESOURCE_HANDLE_H_

#include "Types.hpp"

namespace hercules
{
namespace graphics
{

class ResourceHandle
{
public:
	ResourceHandle() : id_(-1)
	{
	}

	explicit ResourceHandle(int32 id) : id_(id)
	{
	}

	virtual ~ResourceHandle()
	{
	}
	
	int32 getId() const
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
	int32 id_;
};

}

}

#endif /* GRAPHICS_RESOURCE_HANDLE_H_ */
