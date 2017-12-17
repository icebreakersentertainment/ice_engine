#ifndef RENDER_SCENE_HANDLE_H_
#define RENDER_SCENE_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace graphics
{

class RenderSceneHandle : public handles::Handle<RenderSceneHandle>
{
public:
	using handles::Handle<RenderSceneHandle>::Handle;
};

}
}

#endif /* RENDER_SCENE_HANDLE_H_ */
