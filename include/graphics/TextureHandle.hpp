#ifndef TEXTURE_HANDLE_H_
#define TEXTURE_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace graphics
{

class TextureHandle : public handles::Handle<TextureHandle>
{
public:
	using handles::Handle<TextureHandle>::Handle;
};

}
}

#endif /* TEXTURE_HANDLE_H_ */
