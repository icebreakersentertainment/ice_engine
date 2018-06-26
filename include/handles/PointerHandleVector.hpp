#ifndef POINTER_HANDLE_VECTOR_H_
#define POINTER_HANDLE_VECTOR_H_

#include <cstddef>
#include <new>
#include <vector>

#include "Types.hpp"
#include "PointerHandle.hpp"

#include "handles/MemoryPool.hpp"

namespace ice_engine
{
namespace handles
{

template <typename T, typename HandleType>
class PointerHandleVectorIterator;

template <typename T, typename HandleType>
class PointerHandleVectorConstIterator;

template <typename T, typename HandleType>
struct PointerHandleData
{
	HandleType handle;
	T* pointer;
};

template <typename T, typename HandleType>
class PointerHandleVector
{
public:
	PointerHandleVector() = default;

	template <typename ... Args>
	HandleType create(Args&& ... args)
	{
		PointerHandleData<T, HandleType> handleData;
		
		handleData.pointer = memoryPool_.malloc();
		new (handleData.pointer) T(std::forward<Args>(args) ...);
		
		if (freeList_.size() > 0)
		{
			const uint32 index = freeList_.back();
			freeList_.pop_back();
			
			handleData.handle = HandleType(handleData.pointer);
			handleData_[index] = handleData;
		}
		else
		{
			handleData.handle = HandleType(handleData.pointer);
			handleData_.push_back(handleData);
		}
		
		return handleData.handle;
	}
	
	void destroy(const HandleType& handle)
	{
		if (handle.valid())
		{
			for (uint32 i=0; i < handleData_.size(); ++i)
			{
				if (handleData_[i].handle == handle)
				{
					if (valid(i))
					{
						handleData_[i].handle.invalidate();
					
						freeList_.push_back(i);
						
						handleData_[i].pointer->~T();
						
						memoryPool_.free(handleData_[i].pointer);
						handleData_[i].pointer = nullptr;
					}
					break;
				}
			}
		}
	}
	
	bool valid(const HandleType& handle) const
	{
		for (uint32 i=0; i < handleData_.size(); ++i)
		{
			if (handleData_[i].handle == handle)
			{
				return valid(i);
			}
		}
		
		return false;
	}
	
	HandleType handle(const uint32 index) const
	{
		if (valid(index))
		{
			return handleData_[index].handle;
		}
		
		return HandleType();
	}
	
	size_t size() const
	{
		return handleData_.size() - freeList_.size();
	}
	
	void clear()
	{
		handleData_.clear();
		freeList_.clear();
		
		memoryPool_.releaseMemory();
	}
	
	T* get(const HandleType& handle) const
	{
		if (!valid(handle))
		{
			return nullptr;
		}
		
		return handle.pointer;
	}
	
	T& operator[](const HandleType& handle) const
	{
		return *static_cast<T*>(handle.get());
	}
	
	T& operator[](const uint32 index) const
	{
		return *handleData_[index].pointer;
	}
	
	/* Iterator stuff */
	typedef PointerHandleVectorIterator<T, HandleType> iterator;
	typedef PointerHandleVectorConstIterator<T, HandleType> const_iterator;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	
	friend class PointerHandleVectorIterator<T, HandleType>;
	friend class PointerHandleVectorConstIterator<T, HandleType>;
	
	iterator begin()
	{
		return iterator(*this, 0);
	}
 
	iterator end()
	{
		return iterator(*this, handleData_.size());
	}
	
	const_iterator begin() const
	{
		return const_iterator(*this, findIndexOfFirstValidHandle());
	}
 
	const_iterator end() const
	{
		return const_iterator(*this, handleData_.size());
	}
	
	const_iterator cbegin() const
	{
		return const_iterator(*this, findIndexOfFirstValidHandle());
	}
 
	const_iterator cend() const
	{
		return const_iterator(*this, handleData_.size());
	}
 
	
private:
	memory::MemoryPool<T> memoryPool_;
	std::vector<PointerHandleData<T, HandleType>> handleData_;
	std::vector<uint32> freeList_;
	
	uint32 findIndexOfFirstValidHandle() const
	{
		for (uint32 i=0; i < handleData_.size(); ++i)
		{
			if (valid(i))
			{
				return i;
			}
		}
		
		return handleData_.size();
	}
	
	uint32 index(const HandleType& handle) const
	{
		for (uint32 i=0; i < handleData_.size(); ++i)
		{
			if (handleData_[i].handle == handle)
			{
				return i;
			}
		}
		
		return handleData_.size();
	}
	
	bool valid(const uint32 index) const
	{
		return (handleData_[index].pointer != nullptr);
	}
	
	T* get(const uint32 index) const
	{
		if (!valid(index))
		{
			return nullptr;
		}
		
		return handleData_[index].pointer;
	}
};

template <typename T, typename HandleType>
class PointerHandleVectorIterator
{
public:
	PointerHandleVectorIterator(PointerHandleVector<T, HandleType>& handleManager, uint32 index)
		:
		handleManager_(handleManager), index_(index)
	{
		
	}
	
	PointerHandleVectorIterator(const PointerHandleVectorIterator& other)
		:
		handleManager_(other.handleManager_), index_(other.index_)
	{
		
	}
	
	PointerHandleVectorIterator(PointerHandleVectorIterator&& other)
		:
		handleManager_(other.handleManager_), index_(other.index_)
	{
		
	}
	
	bool operator==(PointerHandleVectorIterator other) const
	{
		return index_ == other.index_;
	}
	
	bool operator!=(PointerHandleVectorIterator other) const
	{
		return index_ != other.index_;
	}
	
	T& operator*() const
	{
		return handleManager_[index_];
	}
	
	T* operator->() const
	{
		return &handleManager_[index_];
	}
	
	PointerHandleVectorIterator<T, HandleType>& operator++()
	{
		do
		{
			index_++;
		}
		while (!handleManager_.valid(index_) && index_ < handleManager_.handleData_.size());
		
		return *this;
	}
	
	PointerHandleVectorIterator<T, HandleType> operator++(int)
	{
		PointerHandleVectorIterator<T, HandleType> clone(*this);
		
		do
		{
			index_++;
		}
		while (!handleManager_.valid(index_) && index_ < handleManager_.handleData_.size());
		
		return clone;
	}
	
	HandleType handle() const
	{
		return handleManager_.handle(index_);
	}
	
private:
	PointerHandleVector<T, HandleType>& handleManager_;
	uint32 index_ = 0;
};

template <typename T, typename HandleType>
class PointerHandleVectorConstIterator
{
public:
	PointerHandleVectorConstIterator(const PointerHandleVector<T, HandleType>& handleManager, uint32 index)
		:
		handleManager_(handleManager), index_(index)
	{
		
	}
	
	bool operator==(PointerHandleVectorConstIterator other) const
	{
		return index_ == other.index_;
	}
	
	bool operator!=(PointerHandleVectorConstIterator other) const
	{
		return index_ != other.index_;
	}
	
	const T& operator*() const
	{
		return handleManager_[index_];
	}
	
	const T* operator->() const
	{
		return &handleManager_[index_];
	}
	
	PointerHandleVectorConstIterator<T, HandleType>& operator++()
	{
		const auto size = handleManager_.handleData_.size();
		do
		{
			index_++;
		}
		while (!handleManager_.valid(index_) && index_ < size);
		
		return *this;
	}
	
	PointerHandleVectorConstIterator<T, HandleType> operator++(int)
	{
		PointerHandleVectorConstIterator<T, HandleType> clone(*this);
		
		const auto size = handleManager_.handleData_.size();
		do
		{
			index_++;
		}
		while (!handleManager_.valid(index_) && index_ < size);
		
		return clone;
	}
	
	HandleType handle() const
	{
		return handleManager_.handle(index_);
	}
	
private:
	const PointerHandleVector<T, HandleType>& handleManager_;
	uint32 index_ = 0;
};

}
}

#endif /* POINTER_HANDLE_VECTOR_H_ */

