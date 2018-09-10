#ifndef TERRAIN_RENDERABLE_HANDLE_H_
#define TERRAIN_RENDERABLE_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace graphics
{

class TerrainRenderableHandle : public handles::Handle<TerrainRenderableHandle>
{
public:
	using handles::Handle<TerrainRenderableHandle>::Handle;
};

}
}

#endif /* TERRAIN_RENDERABLE_HANDLE_H_ */
