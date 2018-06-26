#ifndef POLYGON_MESH_HANDLE_H_
#define POLYGON_MESH_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace pathfinding
{

class PolygonMeshHandle : public handles::Handle<PolygonMeshHandle>
{
public:
	using handles::Handle<PolygonMeshHandle>::Handle;
};

}
}

#endif /* POLYGON_MESH_HANDLE_H_ */
