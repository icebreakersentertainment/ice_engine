#include "IceEnginePathfindingAgentStateChangeListener.hpp"

namespace ice_engine
{

IceEnginePathfindingAgentStateChangeListener::IceEnginePathfindingAgentStateChangeListener(ecs::Entity entity, Scene* scene) : entity_(entity), scene_(scene)
{
	
}

IceEnginePathfindingAgentStateChangeListener::~IceEnginePathfindingAgentStateChangeListener()
{
}

void IceEnginePathfindingAgentStateChangeListener::update(const pathfinding::AgentState& agentState)
{
	auto pac = entity_.component<ecs::PathfindingAgentComponent>();

	pac->agentState = agentState;
}


}
