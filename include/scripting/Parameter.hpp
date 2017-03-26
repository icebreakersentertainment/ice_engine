#ifndef SCRIPT_PARAMETER_H_
#define SCRIPT_PARAMETER_H_

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
	TYPE_OBJECT
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
	Parameter() : type_(ParameterType::TYPE_UNKNOWN), value_(Value())
	{
		value_.valuePointer = nullptr;
	};
	
	virtual ~Parameter() {};
	
	template <typename T>
	void valueRef(T& value)
	{
		type_ = ParameterType::TYPE_OBJECT;
		value_.valuePointer = (void*)&value;
	};
	
	template <typename T>
	void value(T value)
	{
		type_ = ParameterType::TYPE_OBJECT;
		value_.valuePointer = (void*)&value;
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
	
};

}
}

#include "scripting/Parameter.inl"

#endif /* SCRIPT_PARAMETER_H_ */
