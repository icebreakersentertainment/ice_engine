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

/**
 * TODO: Make sure we destroy any copied objects if we set a new value for this parameter.
 */
class Parameter
{

public:
	Parameter() : type_(ParameterType::TYPE_UNKNOWN), value_(Value()), sizeOf_(0)
	{
		value_.valuePointer = nullptr;
	};
	
	/**
	 * copy this parameter.
	 * 
	 * Note that if the other Parameter has an object copied by value, the copy constructor
	 * will make another copy of that object by allocating new memory and copying the contents of that object.
	 * 
	 * When this newly created parameter is destroyed, it will call the destructor on that copied object.
	 */
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
	
	/**
	 * If the parameter holds a copy of an object, the destructor for that object will be called.
	 */
	virtual ~Parameter()
	{
		if (type_ == ParameterType::TYPE_OBJECT_VAL)
		{
			destructor_(value_.valuePointer);
		}
	};
	
	/**
	 * Set the parameter by reference.
	 */
	template <typename T>
	void valueRef(T& value)
	{
		type_ = ParameterType::TYPE_OBJECT_REF;
		value_.valuePointer = (void*)&value;
	};
	
	/**
	 * Set the parameter by value. This will make a copy of the passed in value using that values copy constructor.
	 * 
	 * Note that when the parameter object is destroyed, it will call the destructor on the copied object.
	 * 
	 * It is highly recommended that you use relatively simple values.
	 */
	template <typename T>
	void value(T value)
	{
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
