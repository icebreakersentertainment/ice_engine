#ifndef USER_DATA_INLINE_H_
#define USER_DATA_INLINE_H_

namespace hercules
{
namespace physics
{

template <>
inline void UserData::valueRef<bool>(bool& value)
{
	type_ = UserDataType::TYPE_BOOL;
	value_.valueBoolean = value;
};

template <>
inline void UserData::value<bool>(bool value)
{
	type_ = UserDataType::TYPE_BOOL;
	value_.valueBoolean = value;
};

template <>
inline void UserData::valueRef<uint8>(uint8& value)
{
	type_ = UserDataType::TYPE_UINT8;
	value_.valueUint8 = value;
};

template <>
inline void UserData::value<uint8>(uint8 value)
{
	type_ = UserDataType::TYPE_UINT8;
	value_.valueUint8 = value;
};

template <>
inline void UserData::valueRef<int8>(int8& value)
{
	type_ = UserDataType::TYPE_INT8;
	value_.valueInt8 = value;
};

template <>
inline void UserData::value<int8>(int8 value)
{
	type_ = UserDataType::TYPE_INT8;
	value_.valueInt8 = value;
};

template <>
inline void UserData::valueRef<uint16>(uint16& value)
{
	type_ = UserDataType::TYPE_UINT16;
	value_.valueUint16 = value;
};

template <>
inline void UserData::value<uint16>(uint16 value)
{
	type_ = UserDataType::TYPE_UINT16;
	value_.valueUint16 = value;
};

template <>
inline void UserData::valueRef<int16>(int16& value)
{
	type_ = UserDataType::TYPE_INT16;
	value_.valueInt16 = value;
};

template <>
inline void UserData::value<int16>(int16 value)
{
	type_ = UserDataType::TYPE_INT16;
	value_.valueInt16 = value;
};

template <>
inline void UserData::valueRef<uint32>(uint32& value)
{
	type_ = UserDataType::TYPE_UINT32;
	value_.valueUint32 = value;
};

template <>
inline void UserData::value<uint32>(uint32 value)
{
	type_ = UserDataType::TYPE_UINT32;
	value_.valueUint32 = value;
};

template <>
inline void UserData::valueRef<int32>(int32& value)
{
	type_ = UserDataType::TYPE_INT32;
	value_.valueInt32 = value;
};

template <>
inline void UserData::value<int32>(int32 value)
{
	type_ = UserDataType::TYPE_INT32;
	value_.valueInt32 = value;
};

template <>
inline void UserData::valueRef<uint64>(uint64& value)
{
	type_ = UserDataType::TYPE_UINT64;
	value_.valueUint64 = value;
};

template <>
inline void UserData::value<uint64>(uint64 value)
{
	type_ = UserDataType::TYPE_UINT64;
	value_.valueUint64 = value;
};

template <>
inline void UserData::valueRef<int64>(int64& value)
{
	type_ = UserDataType::TYPE_INT64;
	value_.valueInt64 = value;
};

template <>
inline void UserData::value<int64>(int64 value)
{
	type_ = UserDataType::TYPE_INT64;
	value_.valueInt64 = value;
};

template <>
inline void UserData::value<float32>(float32 value)
{
	type_ = UserDataType::TYPE_FLOAT32;
	value_.valueFloat32 = value;
};

template <>
inline void UserData::valueRef<float32>(float32& value)
{
	type_ = UserDataType::TYPE_FLOAT32;
	value_.valueFloat32 = value;
};

template <>
inline void UserData::value<float64>(float64 value)
{
	type_ = UserDataType::TYPE_FLOAT64;
	value_.valueFloat64 = value;
};

template <>
inline void UserData::valueRef<float64>(float64& value)
{
	type_ = UserDataType::TYPE_FLOAT64;
	value_.valueFloat64 = value;
};

template <>
inline bool UserData::value<bool>()
{
	return value_.valueBoolean;
};

template <>
inline bool& UserData::valueRef<bool>()
{
	return value_.valueBoolean;
};

template <>
inline uint8 UserData::value<uint8>()
{
	return value_.valueUint8;
};

template <>
inline uint8& UserData::valueRef<uint8>()
{
	return value_.valueUint8;
};

template <>
inline int8 UserData::value<int8>()
{
	return value_.valueInt8;
};

template <>
inline int8& UserData::valueRef<int8>()
{
	return value_.valueInt8;
};

template <>
inline uint16 UserData::value<uint16>()
{
	return value_.valueUint16;
};

template <>
inline uint16& UserData::valueRef<uint16>()
{
	return value_.valueUint16;
};

template <>
inline int16 UserData::value<int16>()
{
	return value_.valueInt16;
};

template <>
inline uint32 UserData::value<uint32>()
{
	return value_.valueUint32;
};

template <>
inline uint32& UserData::valueRef<uint32>()
{
	return value_.valueUint32;
};

template <>
inline int32 UserData::value<int32>()
{
	return value_.valueInt32;
};

template <>
inline int32& UserData::valueRef<int32>()
{
	return value_.valueInt32;
};

template <>
inline uint64 UserData::value<uint64>()
{
	return value_.valueUint64;
};

template <>
inline int64& UserData::valueRef<int64>()
{
	return value_.valueInt64;
};

template <>
inline int64 UserData::value<int64>()
{
	return value_.valueInt64;
};

template <>
inline float32 UserData::value<float32>()
{
	return value_.valueFloat32;
};

template <>
inline float32& UserData::valueRef<float32>()
{
	return value_.valueFloat32;
};

template <>
inline float64 UserData::value<float64>()
{
	return value_.valueFloat64;
};

template <>
inline float64& UserData::valueRef<float64>()
{
	return value_.valueFloat64;
};

}
}

#endif /* USER_DATA_INLINE_H_ */
