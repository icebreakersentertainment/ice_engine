#ifndef POINTER_HANDLE_H_
#define POINTER_HANDLE_H_

#include <iostream>

#include "Types.hpp"

namespace ice_engine
{
namespace handles
{

class BasePointerHandle {};

template <typename T>
class PointerHandle : BasePointerHandle
{
public:
	PointerHandle() = default;

	explicit PointerHandle(void* object) : object_(object)
	{
	}

	PointerHandle(const PointerHandle& other)
	{
		object_ = other.object_;
	}

	PointerHandle(PointerHandle&& other) noexcept
	{
		object_ = std::exchange(other.object_, nullptr);
	}

	void* get()
	{
		return object_;
	}

//	const void* get() const
//	{
//		return object_;
//	}

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
	
	PointerHandle& operator=(const PointerHandle& other) = default;
	PointerHandle& operator=(PointerHandle&& other) noexcept = default;

	bool operator==(const T& other) const
	{
		return object_ == other.object_;
	}

	bool operator!=(const T& other) const
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

//namespace std
//{
//	template <>
//	struct hash<ice_engine::handles::PointerHandle>
//	{
//		std::size_t operator()(const ice_engine::handles::PointerHandle& k) const noexcept
//		{
//			return hash<const void*>{}(k.get());
//		}
//	};
//}

#endif /* POINTER_HANDLE_H_ */

