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
	bool boolean;
	uint8 uint8;
	int8 int8;
	uint16 uint16;
	int16 int16;
	uint32 uint32;
	int32 int32;
	uint64 uint64;
	int64 int64;
	float32 float32;
	float64 float64;
	void* pointer;
};

class Parameter
{

public:
	Parameter() : type_(ParameterType::TYPE_UNKNOWN), value_(Value())
	{
		value_.pointer = nullptr;
	};
	
	virtual ~Parameter() {};
	
	template <typename T>
	void valueRef(T& value)
	{
		type_ = ParameterType::TYPE_OBJECT;
		value_.pointer = (void*)&value;
	};
	
	template <typename T>
	void value(T value)
	{
		type_ = ParameterType::TYPE_OBJECT;
		value_.pointer = (void*)&value;
	};
	
	template <typename T>
	T value()
	{
		return (*(T*)value_.pointer);
	};
	
	void* pointer() const
	{
		return value_.pointer;
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
