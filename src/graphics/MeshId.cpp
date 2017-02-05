#include "graphics/MeshId.hpp"

namespace hercules
{
namespace graphics
{

const MeshId MeshId::INVALID = MeshId();

MeshId::MeshId() : id_(-1)
{
}

MeshId::MeshId(int32 id) : id_(id)
{
}

MeshId::~MeshId()
{
}

int32 MeshId::getId() const
{
	return id_;
}
	
bool MeshId::operator == (const MeshId &other) const
{
	return id_ == other.id_;
}

bool MeshId::operator != (const MeshId &other) const
{
	return id_ != other.id_;
}

bool MeshId::operator < (const MeshId &other) const
{
	return id_ < other.id_;
}

bool MeshId::operator > (const MeshId &other) const
{
	return other.id_ < id_;
}

bool MeshId::operator <= (const MeshId &other) const
{
	return !(id_ > other.id_);
}

bool MeshId::operator >= (const MeshId &other) const
{
	return !(id_ < other.id_);
}

std::ostream& operator << (std::ostream& os, const MeshId& id)
{
	os << id.id_;
	return os;
}

}
}
