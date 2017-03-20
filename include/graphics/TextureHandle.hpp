#ifndef TEXTURE_HANDLE_H_
#define TEXTURE_HANDLE_H_

#include "ResourceHandle.hpp"

namespace hercules
{
namespace graphics
{

class TextureHandle : public ResourceHandle
{
public:
	using ResourceHandle::ResourceHandle;
	
	static const TextureHandle INVALID;
};

}
}

#endif /* TEXTURE_HANDLE_H_ */
