#ifndef POINTER_HANDLE_H_
#define POINTER_HANDLE_H_

#include <ostream>

#include "Types.hpp"

namespace hercules
{
namespace handles
{

class PointerHandle
{
public:
	PointerHandle() = default;

	PointerHandle(void* object) : object_(object)
	{
	}

	void* get() const
	{
		return object_;
	}
	
	void invalidate()
	{
		object_ = nullptr;
	}
	
	bool valid() const
	{
		return (object_ != nullptr);
	}
	
	explicit operator bool() const
	{
		return valid();
	}
	
	bool operator==(const PointerHandle& other) const
	{
		return object_ == other.object_;
	}

	bool operator!=(const PointerHandle& other) const
	{
		return object_ != other.object_;
	}

protected:
	~PointerHandle() = default;

private:
	void* object_ = nullptr;
};

}
}

#endif /* POINTER_HANDLE_H_ */

