#include "graphics/TextureId.hpp"

namespace graphics
{

const TextureId TextureId::INVALID = TextureId();

TextureId::TextureId() : id_(-1)
{
}

TextureId::TextureId(glm::detail::int32 id) : id_(id)
{
}

TextureId::~TextureId()
{
}

glm::detail::int32 TextureId::getId() const
{
	return id_;
}
	
bool TextureId::operator == (const TextureId &other) const
{
	return id_ == other.id_;
}

bool TextureId::operator != (const TextureId &other) const
{
	return id_ != other.id_;
}

bool TextureId::operator < (const TextureId &other) const
{
	return id_ < other.id_;
}

bool TextureId::operator > (const TextureId &other) const
{
	return other.id_ < id_;
}

bool TextureId::operator <= (const TextureId &other) const
{
	return !(id_ > other.id_);
}

bool TextureId::operator >= (const TextureId &other) const
{
	return !(id_ < other.id_);
}

std::ostream& operator << (std::ostream& os, const TextureId& id)
{
	os << id.id_;
	return os;
}

}
