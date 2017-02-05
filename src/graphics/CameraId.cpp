#include "graphics/CameraId.hpp"

namespace hercules
{
namespace graphics
{

const CameraId CameraId::INVALID = CameraId();

CameraId::CameraId() : id_(-1)
{
}

CameraId::CameraId(int32 id) : id_(id)
{
}

CameraId::~CameraId()
{
}

int32 CameraId::getId() const
{
	return id_;
}
	
bool CameraId::operator == (const CameraId &other) const
{
	return id_ == other.id_;
}

bool CameraId::operator != (const CameraId &other) const
{
	return id_ != other.id_;
}

bool CameraId::operator < (const CameraId &other) const
{
	return id_ < other.id_;
}

bool CameraId::operator > (const CameraId &other) const
{
	return other.id_ < id_;
}

bool CameraId::operator <= (const CameraId &other) const
{
	return !(id_ > other.id_);
}

bool CameraId::operator >= (const CameraId &other) const
{
	return !(id_ < other.id_);
}

std::ostream& operator << (std::ostream& os, const CameraId& id)
{
	os << id.id_;
	return os;
}

}
}
