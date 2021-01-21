#ifndef HANDLE_H_
#define HANDLE_H_

#include <ostream>
#include <utility>

#include <boost/type_index.hpp>

#include "Types.hpp"

namespace ice_engine
{
namespace handles
{

class BaseHandle {};

template <typename T>
class Handle : BaseHandle
{
public:
	Handle() = default;

	Handle(const Handle& other)
	{
		id_ = other.id_;
	}

	Handle(Handle&& other) noexcept
	{
		id_ = std::exchange(other.id_, 0);
	}

	explicit Handle(uint64 id) : id_(id)
	{
	}

	Handle(uint32 index, uint32 version) : id_(uint64(index) | uint64(version) << 32UL)
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
	
	bool valid() const
	{
		return (id_ != INVALID && version() != 0);
	}
	
	explicit operator bool() const
	{
		return valid();
	}

	Handle& operator=(const Handle& other) = default;
	Handle& operator=(Handle&& other) noexcept = default;

	bool operator==(const T& other) const
	{
		return id_ == other.id_;
	}

	bool operator!=(const T& other) const
	{
		return id_ != other.id_;
	}

	bool operator<(const T& other) const
	{
		return id_ < other.id_;
	}

	bool operator>(const T& other) const
	{
		return other.id_ < id_;
	}

	bool operator<=(const T& other) const
	{
		return !(id_ > other.id_);
	}

	bool operator>=(const T& other) const
	{
		return !(id_ < other.id_);
	}

	friend std::ostream& operator<<(std::ostream& os, const T& other)
	{
		os << "(" << boost::typeindex::type_id<Handle>().pretty_name() << ")["
           << "id: " << other.id_
           << ", index: " << other.index()
           << ", version: " << other.version()
           << "]"
           ;

		return os;
	}

	static constexpr uint64 INVALID = 0;

protected:
	~Handle() = default;

private:
	// Index stored in the hi 32bits, and version stored in the lo 32bits.
	uint64 id_ = INVALID;
};

}
}

#endif /* HANDLE_H_ */

