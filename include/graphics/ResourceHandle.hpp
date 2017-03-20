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
	
	bool operator == (const ResourceHandle &other) const
	{
		return id_ == other.id_;
	}

	bool operator != (const ResourceHandle &other) const
	{
		return id_ != other.id_;
	}

	bool operator < (const ResourceHandle &other) const
	{
		return id_ < other.id_;
	}

	bool operator > (const ResourceHandle &other) const
	{
		return other.id_ < id_;
	}

	bool operator <= (const ResourceHandle &other) const
	{
		return !(id_ > other.id_);
	}

	bool operator >= (const ResourceHandle &other) const
	{
		return !(id_ < other.id_);
	}
/*
	bool operator << (const ResourceHandle &other) const;
	
	friend std::ostream& operator << (std::ostream& os, const ResourceHandle& id);
*/
private:
	int32 id_;
};

/*
std::ostream& operator << (std::ostream& os, const ResourceHandle& id)
{
	os << id.id_;
	return os;
}
*/

}

}

#endif /* GRAPHICS_RESOURCE_HANDLE_H_ */
