#ifndef TEXTURE_ID_H_
#define TEXTURE_ID_H_

#include <iostream>

#include "Types.hpp"

namespace hercules
{
namespace graphics
{

class TextureId
{
public:
	TextureId();
	explicit TextureId(int32 id);
	// We make the destructor non-virtual so that we can't extend this class easily
	~TextureId();
	
	int32 getId() const;
	
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
	int32 id_;
};

}

}

#endif /* TEXTURE_ID_H_ */
