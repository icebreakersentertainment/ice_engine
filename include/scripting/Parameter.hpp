#ifndef SCRIPT_PARAMETER_H_
#define SCRIPT_PARAMETER_H_

#include <functional>
#include <type_traits>

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

class Parameter
{
// Only allowed for non-integral types
//static_assert(std::is_integral<T>::value, "T must be a non-integral type");
public:
	Parameter() : type_(ParameterType::TYPE_UNKNOWN), valuePtr_(nullptr) {};
	virtual ~Parameter() {};
	
	template <typename T>
	void value(T& value)
	{
		type_ = ParameterType::TYPE_OBJECT;
		valuePtr_ = (void*)value;
	};
	
	template <>
	void value<uint8>(uint8& value)
	{
		type_ = ParameterType::TYPE_UINT8;
		valuePtr_ = (void*)value;
	};
	
	template <typename T>
	void value(T value)
	{
		type_ = ParameterType::TYPE_OBJECT;
		valuePtr_ = (void*)&value;
	};
	
	template <>
	void value<uint8>(uint8 value)
	{
		type_ = ParameterType::TYPE_UINT8;
		valuePtr_ = (void*)&value;
	};
	
	template <>
	void value<float32>(float32 value)
	{
		type_ = ParameterType::TYPE_FLOAT32;
		valuePtr_ = (void*)&value;
	};
	
	template <typename T>
	T value()
	{
		return (*(T*)valuePtr_);
	};
	
	template <>
	uint8 value()
	{
		return *(uint8*)valuePtr_;
	};
	
	void* pointer() const
	{
		return valuePtr_;
	};
	
	ParameterType type() const
	{
		return type_;
	}
	

private:
	ParameterType type_;
	void* valuePtr_;
};

}
}

#endif /* SCRIPT_PARAMETER_H_ */
