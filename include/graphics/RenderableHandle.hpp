#ifndef RENDERABLE_HANDLE_H_
#define RENDERABLE_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace graphics
{

class RenderableHandle : public handles::Handle<RenderableHandle>
{
public:
	using handles::Handle<RenderableHandle>::Handle;
};

}
}

#endif /* RENDERABLE_HANDLE_H_ */
