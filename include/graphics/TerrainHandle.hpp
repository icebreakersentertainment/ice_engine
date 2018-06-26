#ifndef TERRAIN_HANDLE_H_
#define TERRAIN_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace graphics
{

class TerrainHandle : public handles::Handle<TerrainHandle>
{
public:
	using handles::Handle<TerrainHandle>::Handle;
};

}
}

#endif /* TERRAIN_HANDLE_H_ */
