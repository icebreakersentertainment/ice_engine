#ifndef HANDLE_H_
#define HANDLE_H_

#include <ostream>

#include "Types.hpp"

namespace hercules
{
namespace handles
{

class Handle
{
public:
	Handle() : id_(0)
	{
	}

	explicit Handle(uint64 id) : id_(id)
	{
	}

	Handle(uint32 index, uint32 version) : id_(uint64(index) | uint64(version) << 32UL)
	{
	}

	virtual ~Handle()
	{
	}
	
	uint64 id() const
	{
		return id_;
	}
	
	uint32 index() const
	{
		return id_ & 0xffffffffUL;
	}
	
	uint32 version() const
	{
		return id_ >> 32;
	}
	
	void invalidate()
	{
		id_ = uint64(uint64(index()) | uint64(0) << 32UL);
	}
	
	bool operator==(const Handle& other) const
	{
		return id_ == other.id_;
	}

	bool operator!=(const Handle& other) const
	{
		return id_ != other.id_;
	}

	bool operator<(const Handle& other) const
	{
		return id_ < other.id_;
	}

	bool operator>(const Handle& other) const
	{
		return other.id_ < id_;
	}

	bool operator<=(const Handle& other) const
	{
		return !(id_ > other.id_);
	}

	bool operator>=(const Handle& other) const
	{
		return !(id_ < other.id_);
	}

	friend std::ostream& operator<<(std::ostream& os, const Handle& other)
	{
		os << "Id: " << other.id_ << ", Index: " << other.index() << ", Version: " << other.version();
		return os;
	}

	static const Handle INVALID;
private:
	// Index stored in the hi 32bits, and version stored in the lo 32bits.
	uint64 id_;
};

}
}

#endif /* HANDLE_H_ */

