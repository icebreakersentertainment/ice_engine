#ifndef SKYBOX_RENDERABLE_HANDLE_H_
#define SKYBOX_RENDERABLE_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace graphics
{

class SkyboxRenderableHandle : public handles::Handle<SkyboxRenderableHandle>
{
public:
	using handles::Handle<SkyboxRenderableHandle>::Handle;
};

}
}

#endif /* SKYBOX_RENDERABLE_HANDLE_H_ */
