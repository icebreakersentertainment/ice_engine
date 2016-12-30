#ifndef TEXTURE_ID_H_
#define TEXTURE_ID_H_

#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace graphics
{

class TextureId
{
public:
	TextureId();
	explicit TextureId(glm::detail::int32 id);
	// We make the destructor non-virtual so that we can't extend this class easily
	~TextureId();
	
	glm::detail::int32 getId() const;
	
	bool operator == (const TextureId &other) const;
	bool operator != (const TextureId &other) const;
	bool operator < (const TextureId &other) const;
	bool operator > (const TextureId &other) const;
	bool operator <= (const TextureId &other) const;
	bool operator >= (const TextureId &other) const;
	bool operator << (const TextureId &other) const;
	
	friend std::ostream& operator << (std::ostream& os, const TextureId& id);
	
	static const TextureId INVALID;

private:
	glm::detail::int32 id_;
};

}

#endif /* TEXTURE_ID_H_ */
