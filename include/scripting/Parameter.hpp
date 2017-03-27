#ifndef SCRIPT_PARAMETER_H_
#define SCRIPT_PARAMETER_H_

#include <functional>
#include "Types.hpp"

namespace hercules
{
namespace scripting
{

enum ParameterType
{
	TYPE_UNKNOWN = -1,
	TYPE_BOOL,
	TYPE_INT8,
	TYPE_UINT8,
	TYPE_INT16,
	TYPE_UINT16,
	TYPE_INT32,
	TYPE_UINT32,
	TYPE_INT64,
	TYPE_UINT64,
	TYPE_FLOAT32,
	TYPE_FLOAT64,
	TYPE_OBJECT_REF,
	TYPE_OBJECT_VAL
};

union Value
{
	bool valueBoolean;
	uint8 valueUint8;
	int8 valueInt8;
	uint16 valueUint16;
	int16 valueInt16;
	uint32 valueUint32;
	int32 valueInt32;
	uint64 valueUint64;
	int64 valueInt64;
	float32 valueFloat32;
	float64 valueFloat64;
	void* valuePointer;
};

class Parameter
{

public:
	Parameter() : type_(ParameterType::TYPE_UNKNOWN), value_(Value()), sizeOf_(0)
	{
		value_.valuePointer = nullptr;
	};
	
	Parameter(const Parameter& other)
	{
		type_ = other.type_;
		value_ = other.value_;
		sizeOf_ = other.sizeOf_;
		destructor_ = other.destructor_;
		
		if (type_ == ParameterType::TYPE_OBJECT_VAL)
		{
			void* p = malloc(sizeOf_);
			memcpy(p, value_.valuePointer, sizeOf_);
			value_.valuePointer = p;
		}
	};
	
	virtual ~Parameter()
	{
		if (type_ == ParameterType::TYPE_OBJECT_VAL)
		{
			destructor_(value_.valuePointer);
		}
	};
	
	template <typename T>
	void valueRef(T& value)
	{
		type_ = ParameterType::TYPE_OBJECT_REF;
		value_.valuePointer = (void*)&value;
	};
	
	template <typename T>
	void value(T value)
	{
		// static_assert(false, "Cannot set object by value.");
		
		type_ = ParameterType::TYPE_OBJECT_VAL;
		
		T* p = new T(value);
		value_.valuePointer = (void*)p;
		sizeOf_ = sizeof(T);
		destructor_ = [](const void* x) { static_cast<const T*>(x)->~T(); };
	};
	
	template <typename T>
	T& valueRef()
	{
		return (*(T*)value_.valuePointer);
	};
	
	template <typename T>
	T value()
	{
		return (*(T*)value_.valuePointer);
	};
	
	void* pointer() const
	{
		return value_.valuePointer;
	};
	
	ParameterType type() const
	{
		return type_;
	}
	

private:
	ParameterType type_;
	Value value_;
	size_t sizeOf_;
	std::function<void(const void*)> destructor_;
	
};

}
}

#include "scripting/Parameter.inl"

#endif /* SCRIPT_PARAMETER_H_ */
