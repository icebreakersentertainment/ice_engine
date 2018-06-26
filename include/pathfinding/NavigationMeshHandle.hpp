#ifndef NAVIGATION_MESH_HANDLE_H_
#define NAVIGATION_MESH_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace pathfinding
{

class NavigationMeshHandle : public handles::Handle<NavigationMeshHandle>
{
public:
	using handles::Handle<NavigationMeshHandle>::Handle;
};

}
}

#endif /* NAVIGATION_MESH_HANDLE_H_ */
