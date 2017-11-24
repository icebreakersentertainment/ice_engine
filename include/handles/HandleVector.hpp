#ifndef HANDLE_VECTOR_H_
#define HANDLE_VECTOR_H_

#include <cstddef>
#include <new>
#include <vector>

#include "Types.hpp"
#include "Handle.hpp"

#include "handles/MemoryPool.hpp"

namespace hercules
{
namespace handles
{

template <typename T, typename HandleType>
class HandleVectorIterator;

template <typename T, typename HandleType>
class HandleVectorConstIterator;

template <typename T, typename HandleType>
struct HandleData
{
	HandleType handle;
	T* pointer;
};

template <typename T, typename HandleType>
class HandleVector
{
public:
	HandleVector() = default;

	template <typename ... Args>
	HandleType create(Args ... args)
	{
		HandleData<T, HandleType> handleData;
		
		handleData.pointer = memoryPool_.malloc();
		new (handleData.pointer) T(std::forward<Args>(args) ...);
		
		if (freeList_.size() > 0)
		{
			const uint32 index = freeList_.back();
			freeList_.pop_back();
			
			handleData.handle = HandleType(index, currentVersion_);
			handleData_[index] = handleData;
		}
		else
		{
			handleData.handle = HandleType(handleData_.size(), currentVersion_);
			handleData_.push_back(handleData);
		}
		
		currentVersion_++;
		
		return handleData.handle;
	}
	
	void destroy(const HandleType& handle)
	{
		if (valid(handle))
		{
			const uint32 index = handle.index();
			auto& handleData = handleData_[index];
			handleData.handle.invalidate();
			
			freeList_.push_back(index);
			
			handleData.pointer->~T();
			
			memoryPool_.free(handleData.pointer);
			handleData.pointer = nullptr;
		}
	}
	
	bool valid(const HandleType& handle) const
	{
		const auto index = handle.index();
		return (index < handleData_.size() && handleData_[index].handle == handle && handle.version() != 0);
	}
	
	HandleType handle(const uint64 id) const
	{
		auto h = HandleType(id);
		
		if (valid(h))
		{
			return handleData_[h.index()].handle;
		}
		
		return HandleType();
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
		
		return handleData_[handle.index()].pointer;
	}
	
	T& operator[](const HandleType& handle) const
	{
		return *handleData_[handle.index()].pointer;
	}
	
	T& operator[](const uint32 index) const
	{
		return *handleData_[index].pointer;
	}
	
	/* Iterator stuff */
	typedef HandleVectorIterator<T, HandleType> iterator;
	typedef HandleVectorConstIterator<T, HandleType> const_iterator;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	
	friend class HandleVectorIterator<T, HandleType>;
	friend class HandleVectorConstIterator<T, HandleType>;
	
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
		return const_iterator(*this, 0);
	}
 
	const_iterator end() const
	{
		return const_iterator(*this, handleData_.size());
	}
	
	const_iterator cbegin() const
	{
		return const_iterator(*this, 0);
	}
 
	const_iterator cend() const
	{
		return const_iterator(*this, handleData_.size());
	}
 
	
private:
	memory::MemoryPool<T> memoryPool_;
	std::vector<HandleData<T, HandleType>> handleData_;
	std::vector<uint32> freeList_;
	
	uint32 currentVersion_ = 1;
	
	bool valid(const uint32 index) const
	{
		return (handleData_[index].handle.version() != 0);
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
class HandleVectorIterator
{
public:
	HandleVectorIterator(HandleVector<T, HandleType>& handleManager, size_t index)
		:
		handleManager_(handleManager), index_(index)
	{
		
	}
	
	bool operator==(HandleVectorIterator other) const
	{
		return index_ == other.index_;
	}
	
	bool operator!=(HandleVectorIterator other) const
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
	
	HandleVectorIterator<T, HandleType>& operator++()
	{
		do
		{
			index_++;
		}
		while (!handleManager_.valid(index_) && index_ < handleManager_.handleData_.size());
		
		return *this;
	}
	
	HandleVectorIterator<T, HandleType> operator++(int)
	{
		HandleVectorIterator<T, HandleType> clone(*this);
		
		do
		{
			index_++;
		}
		while (!handleManager_.valid(index_) && index_ < handleManager_.handleData_.size());
		
		return clone;
	}
	
private:
	HandleVector<T, HandleType>& handleManager_;
	size_t index_ = 0;
};

template <typename T, typename HandleType>
class HandleVectorConstIterator
{
public:
	HandleVectorConstIterator(const HandleVector<T, HandleType>& handleManager, size_t index)
		:
		handleManager_(handleManager), index_(index)
	{
		
	}
	
	bool operator==(HandleVectorConstIterator other) const
	{
		return index_ == other.index_;
	}
	
	bool operator!=(HandleVectorConstIterator other) const
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
	
	HandleVectorConstIterator<T, HandleType>& operator++()
	{
		const auto size = handleManager_.handleData_.size();
		do
		{
			index_++;
		}
		while (!handleManager_.valid(index_) && index_ < size);
		
		return *this;
	}
	
	HandleVectorConstIterator<T, HandleType> operator++(int)
	{
		HandleVectorConstIterator<T, HandleType> clone(*this);
		
		const auto size = handleManager_.handleData_.size();
		do
		{
			index_++;
		}
		while (!handleManager_.valid(index_) && index_ < size);
		
		return clone;
	}
	
private:
	const HandleVector<T, HandleType>& handleManager_;
	size_t index_ = 0;
};

}
}

#endif /* HANDLE_VECTOR_H_ */

