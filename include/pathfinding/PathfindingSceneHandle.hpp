#ifndef PATHFINDING_SCENE_HANDLE_H_
#define PATHFINDING_SCENE_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
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

#endif /* PATHFINDING_SCENE_HANDLE_H_ */
