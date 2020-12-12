#ifndef PATHFINDINGCROWDCOMPONENT_H_
#define PATHFINDINGCROWDCOMPONENT_H_

#include "pathfinding/CrowdHandle.hpp"
#include "pathfinding/NavigationMeshHandle.hpp"
#include "pathfinding/CrowdConfig.hpp"

#include "ecs/Serialization.hpp"

namespace ice_engine
{
namespace ecs
{

struct PathfindingCrowdComponent
{
	PathfindingCrowdComponent() = default;
	
	PathfindingCrowdComponent(
		pathfinding::NavigationMeshHandle navigationMeshHandle,
		pathfinding::CrowdConfig crowdConfig = pathfinding::CrowdConfig(),
		pathfinding::CrowdHandle crowdHandle = pathfinding::CrowdHandle()
	)
	:
		navigationMeshHandle(navigationMeshHandle),
		crowdConfig(crowdConfig),
		crowdHandle(crowdHandle)
	{

	};

	static uint8 id()  { return 10; }

	pathfinding::NavigationMeshHandle navigationMeshHandle;
	pathfinding::CrowdConfig crowdConfig;
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
	ar & c.navigationMeshHandle & c.crowdConfig & c.crowdHandle;
}

}
}

#endif /* PATHFINDINGCROWDCOMPONENT_H_ */
