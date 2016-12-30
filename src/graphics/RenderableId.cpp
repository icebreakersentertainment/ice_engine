#include "graphics/RenderableId.hpp"

namespace graphics
{

const RenderableId RenderableId::INVALID = RenderableId();

RenderableId::RenderableId() : id_(-1)
{
}

RenderableId::RenderableId(glm::detail::int32 id) : id_(id)
{
}

RenderableId::~RenderableId()
{
}

glm::detail::int32 RenderableId::getId() const
{
	return id_;
}
	
bool RenderableId::operator == (const RenderableId &other) const
{
	return id_ == other.id_;
}

bool RenderableId::operator != (const RenderableId &other) const
{
	return id_ != other.id_;
}

bool RenderableId::operator < (const RenderableId &other) const
{
	return id_ < other.id_;
}

bool RenderableId::operator > (const RenderableId &other) const
{
	return other.id_ < id_;
}

bool RenderableId::operator <= (const RenderableId &other) const
{
	return !(id_ > other.id_);
}

bool RenderableId::operator >= (const RenderableId &other) const
{
	return !(id_ < other.id_);
}

std::ostream& operator << (std::ostream& os, const RenderableId& id)
{
	os << id.id_;
	return os;
}

}
