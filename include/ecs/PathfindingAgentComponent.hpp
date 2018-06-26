#ifndef PATHFINDINGAGENTCOMPONENT_H_
#define PATHFINDINGAGENTCOMPONENT_H_

#include "pathfinding/CrowdHandle.hpp"
#include "pathfinding/AgentHandle.hpp"

namespace ice_engine
{
namespace ecs
{

struct PathfindingAgentComponent
{
	PathfindingAgentComponent() = default;
	
	PathfindingAgentComponent(pathfinding::CrowdHandle crowdHandle, pathfinding::AgentHandle agentHandle) : crowdHandle(crowdHandle), agentHandle(agentHandle)
	{
	};
	
	pathfinding::CrowdHandle crowdHandle;
	pathfinding::AgentHandle agentHandle;
};

}
}

#endif /* PATHFINDINGAGENTCOMPONENT_H_ */
