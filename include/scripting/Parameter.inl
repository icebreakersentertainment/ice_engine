#ifndef SCRIPT_PARAMETER_INLINE_H_
#define SCRIPT_PARAMETER_INLINE_H_

namespace hercules
{
namespace scripting
{

template <>
inline void Parameter::valueRef<bool>(bool& value)
{
	type_ = ParameterType::TYPE_BOOL;
	value_.valueBoolean = value;
};

template <>
inline void Parameter::value<bool>(bool value)
{
	type_ = ParameterType::TYPE_BOOL;
	value_.valueBoolean = value;
};

template <>
inline void Parameter::valueRef<uint8>(uint8& value)
{
	type_ = ParameterType::TYPE_UINT8;
	value_.valueUint8 = value;
};

template <>
inline void Parameter::value<uint8>(uint8 value)
{
	type_ = ParameterType::TYPE_UINT8;
	value_.valueUint8 = value;
};

template <>
inline void Parameter::valueRef<int8>(int8& value)
{
	type_ = ParameterType::TYPE_INT8;
	value_.valueInt8 = value;
};

template <>
inline void Parameter::value<int8>(int8 value)
{
	type_ = ParameterType::TYPE_INT8;
	value_.valueInt8 = value;
};

template <>
inline void Parameter::valueRef<uint16>(uint16& value)
{
	type_ = ParameterType::TYPE_UINT16;
	value_.valueUint16 = value;
};

template <>
inline void Parameter::value<uint16>(uint16 value)
{
	type_ = ParameterType::TYPE_UINT16;
	value_.valueUint16 = value;
};

template <>
inline void Parameter::valueRef<int16>(int16& value)
{
	type_ = ParameterType::TYPE_INT16;
	value_.valueInt16 = value;
};

template <>
inline void Parameter::value<int16>(int16 value)
{
	type_ = ParameterType::TYPE_INT16;
	value_.valueInt16 = value;
};

template <>
inline void Parameter::valueRef<uint32>(uint32& value)
{
	type_ = ParameterType::TYPE_UINT32;
	value_.valueUint32 = value;
};

template <>
inline void Parameter::value<uint32>(uint32 value)
{
	type_ = ParameterType::TYPE_UINT32;
	value_.valueUint32 = value;
};

template <>
inline void Parameter::valueRef<int32>(int32& value)
{
	type_ = ParameterType::TYPE_INT32;
	value_.valueInt32 = value;
};

template <>
inline void Parameter::value<int32>(int32 value)
{
	type_ = ParameterType::TYPE_INT32;
	value_.valueInt32 = value;
};

template <>
inline void Parameter::valueRef<uint64>(uint64& value)
{
	type_ = ParameterType::TYPE_UINT64;
	value_.valueUint64 = value;
};

template <>
inline void Parameter::value<uint64>(uint64 value)
{
	type_ = ParameterType::TYPE_UINT64;
	value_.valueUint64 = value;
};

template <>
inline void Parameter::valueRef<int64>(int64& value)
{
	type_ = ParameterType::TYPE_INT64;
	value_.valueInt64 = value;
};

template <>
inline void Parameter::value<int64>(int64 value)
{
	type_ = ParameterType::TYPE_INT64;
	value_.valueInt64 = value;
};

template <>
inline void Parameter::value<float32>(float32 value)
{
	type_ = ParameterType::TYPE_FLOAT32;
	value_.valueFloat32 = value;
};

template <>
inline void Parameter::valueRef<float32>(float32& value)
{
	type_ = ParameterType::TYPE_FLOAT32;
	value_.valueFloat32 = value;
};

template <>
inline void Parameter::value<float64>(float64 value)
{
	type_ = ParameterType::TYPE_FLOAT64;
	value_.valueFloat64 = value;
};

template <>
inline void Parameter::valueRef<float64>(float64& value)
{
	type_ = ParameterType::TYPE_FLOAT64;
	value_.valueFloat64 = value;
};

template <>
inline bool Parameter::value<bool>()
{
	return value_.valueBoolean;
};

template <>
inline bool& Parameter::valueRef<bool>()
{
	return value_.valueBoolean;
};

template <>
inline uint8 Parameter::value<uint8>()
{
	return value_.valueUint8;
};

template <>
inline uint8& Parameter::valueRef<uint8>()
{
	return value_.valueUint8;
};

template <>
inline int8 Parameter::value<int8>()
{
	return value_.valueInt8;
};

template <>
inline int8& Parameter::valueRef<int8>()
{
	return value_.valueInt8;
};

template <>
inline uint16 Parameter::value<uint16>()
{
	return value_.valueUint16;
};

template <>
inline uint16& Parameter::valueRef<uint16>()
{
	return value_.valueUint16;
};

template <>
inline int16 Parameter::value<int16>()
{
	return value_.valueInt16;
};

template <>
inline uint32 Parameter::value<uint32>()
{
	return value_.valueUint32;
};

template <>
inline uint32& Parameter::valueRef<uint32>()
{
	return value_.valueUint32;
};

template <>
inline int32 Parameter::value<int32>()
{
	return value_.valueInt32;
};

template <>
inline int32& Parameter::valueRef<int32>()
{
	return value_.valueInt32;
};

template <>
inline uint64 Parameter::value<uint64>()
{
	return value_.valueUint64;
};

template <>
inline int64& Parameter::valueRef<int64>()
{
	return value_.valueInt64;
};

template <>
inline int64 Parameter::value<int64>()
{
	return value_.valueInt64;
};

template <>
inline float32 Parameter::value<float32>()
{
	return value_.valueFloat32;
};

template <>
inline float32& Parameter::valueRef<float32>()
{
	return value_.valueFloat32;
};

template <>
inline float64 Parameter::value<float64>()
{
	return value_.valueFloat64;
};

template <>
inline float64& Parameter::valueRef<float64>()
{
	return value_.valueFloat64;
};

}
}

#endif /* SCRIPT_PARAMETER_INLINE_H_ */
