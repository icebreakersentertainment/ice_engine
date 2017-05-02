#ifndef RENDERABLE_HANDLE_H_
#define RENDERABLE_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace graphics
{

class RenderableHandle : public handles::Handle
{
public:
	using handles::Handle::Handle;
	
	static const RenderableHandle INVALID;
};

}
}

#endif /* RENDERABLE_HANDLE_H_ */
