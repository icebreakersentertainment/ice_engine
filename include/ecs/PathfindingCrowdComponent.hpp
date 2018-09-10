#ifndef PATHFINDINGCROWDCOMPONENT_H_
#define PATHFINDINGCROWDCOMPONENT_H_

#include "pathfinding/CrowdHandle.hpp"
#include "pathfinding/NavigationMeshHandle.hpp"

#include "ecs/Serialization.hpp"

namespace ice_engine
{
namespace ecs
{

struct PathfindingCrowdComponent
{
	PathfindingCrowdComponent() = default;
	
	PathfindingCrowdComponent(
		pathfinding::NavigationMeshHandle&& navigationMeshHandle
	)
	:  navigationMeshHandle(std::forward<pathfinding::NavigationMeshHandle>(navigationMeshHandle))
	{
	};
	
	PathfindingCrowdComponent(
		pathfinding::NavigationMeshHandle&& navigationMeshHandle,
		pathfinding::CrowdHandle&& crowdHandle
	)
	:  navigationMeshHandle(std::forward<pathfinding::NavigationMeshHandle>(navigationMeshHandle)),
	   crowdHandle(std::forward<pathfinding::CrowdHandle>(crowdHandle))
	{
	};

	pathfinding::NavigationMeshHandle navigationMeshHandle;
	pathfinding::CrowdHandle crowdHandle;
};

}
}

namespace boost
{
namespace serialization
{

template<class Archive>
void serialize(Archive& ar, ice_engine::ecs::PathfindingCrowdComponent& c, const unsigned int version)
{
	ar & c.navigationMeshHandle & c.crowdHandle;
}

}
}

#endif /* PATHFINDINGCROWDCOMPONENT_H_ */
