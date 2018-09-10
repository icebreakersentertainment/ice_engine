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

namespace std
{
	template <>
	struct hash<ice_engine::pathfinding::NavigationMeshHandle>
	{
		std::size_t operator()(const ice_engine::pathfinding::NavigationMeshHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* NAVIGATION_MESH_HANDLE_H_ */
