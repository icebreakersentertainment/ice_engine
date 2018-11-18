#ifndef PATHFINDINGAGENTCOMPONENT_H_
#define PATHFINDINGAGENTCOMPONENT_H_

#include "pathfinding/CrowdHandle.hpp"
#include "pathfinding/AgentHandle.hpp"
#include "pathfinding/AgentParams.hpp"
#include "pathfinding/AgentState.hpp"
#include "pathfinding/MovementRequestState.hpp"

#include "ecs/Serialization.hpp"

namespace ice_engine
{
namespace ecs
{

struct PathfindingAgentComponent
{
	PathfindingAgentComponent() = default;
	
	PathfindingAgentComponent(
		pathfinding::CrowdHandle crowdHandle,
		pathfinding::AgentHandle agentHandle,
		pathfinding::AgentParams agentParams = pathfinding::AgentParams(),
		glm::vec3 target = glm::vec3())
	:
		crowdHandle(crowdHandle),
		agentHandle(agentHandle),
		agentParams(agentParams),
		target(target)
	{
	};
	
	PathfindingAgentComponent(
		pathfinding::CrowdHandle crowdHandle,
		pathfinding::AgentParams agentParams,
		glm::vec3 target)
	:
		crowdHandle(crowdHandle),
		agentParams(agentParams),
		movementRequestState(pathfinding::MovementRequestState::REQUESTING),
		target(target)
	{
	};

	static uint8 id()  { return 11; }

	pathfinding::CrowdHandle crowdHandle;
	pathfinding::AgentHandle agentHandle;
	pathfinding::AgentParams agentParams;
	pathfinding::AgentState  agentState = pathfinding::AgentState::INVALID;
	pathfinding::MovementRequestState movementRequestState = pathfinding::MovementRequestState::NONE;
	glm::vec3 target;
};

}
}

namespace boost
{
namespace serialization
{

template<class Archive>
void serialize(Archive& ar, ice_engine::ecs::PathfindingAgentComponent& c, const unsigned int version)
{
	ar & c.crowdHandle & c.agentHandle & c.agentParams & c.agentState & c.movementRequestState & c.target;
}

}
}

#endif /* PATHFINDINGAGENTCOMPONENT_H_ */
