#ifndef PATHFINDING_SCENE_HANDLE_H_
#define PATHFINDING_SCENE_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace pathfinding
{

class PathfindingSceneHandle : public handles::Handle<PathfindingSceneHandle>
{
public:
	using handles::Handle<PathfindingSceneHandle>::Handle;
};

}
}

namespace std
{
	template <>
	struct hash<ice_engine::pathfinding::PathfindingSceneHandle>
	{
		std::size_t operator()(const ice_engine::pathfinding::PathfindingSceneHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* PATHFINDING_SCENE_HANDLE_H_ */
