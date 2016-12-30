#include "graphics/SkeletonId.hpp"

namespace graphics
{

const SkeletonId SkeletonId::INVALID = SkeletonId();

SkeletonId::SkeletonId() : id_(-1)
{
}

SkeletonId::SkeletonId(glm::detail::int32 id) : id_(id)
{
}

SkeletonId::~SkeletonId()
{
}

glm::detail::int32 SkeletonId::getId() const
{
	return id_;
}
	
bool SkeletonId::operator == (const SkeletonId &other) const
{
	return id_ == other.id_;
}

bool SkeletonId::operator != (const SkeletonId &other) const
{
	return id_ != other.id_;
}

bool SkeletonId::operator < (const SkeletonId &other) const
{
	return id_ < other.id_;
}

bool SkeletonId::operator > (const SkeletonId &other) const
{
	return other.id_ < id_;
}

bool SkeletonId::operator <= (const SkeletonId &other) const
{
	return !(id_ > other.id_);
}

bool SkeletonId::operator >= (const SkeletonId &other) const
{
	return !(id_ < other.id_);
}

std::ostream& operator << (std::ostream& os, const SkeletonId& id)
{
	os << id.id_;
	return os;
}

}
