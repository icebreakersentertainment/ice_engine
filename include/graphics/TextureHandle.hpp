#ifndef TEXTURE_HANDLE_H_
#define TEXTURE_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace graphics
{

class TextureHandle : public handles::Handle
{
public:
	using handles::Handle::Handle;
	
	static const TextureHandle INVALID;
};

}
}

#endif /* TEXTURE_HANDLE_H_ */
