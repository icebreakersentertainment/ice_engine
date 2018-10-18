#include "IceEnginePathfindingAgentStateChangeListener.hpp"

namespace ice_engine
{

IceEnginePathfindingAgentStateChangeListener::IceEnginePathfindingAgentStateChangeListener(ecs::Entity entity) : entity_(entity)
{
	
}

IceEnginePathfindingAgentStateChangeListener::~IceEnginePathfindingAgentStateChangeListener()
{
}

void IceEnginePathfindingAgentStateChangeListener::update(const pathfinding::AgentState& agentState)
{
	std::cout << "ASDF " << agentState  << std::endl;
	auto pac = entity_.component<ecs::PathfindingAgentComponent>();

	pac->agentState = agentState;

	if (entity_.hasComponent<ecs::DirtyComponent>())
	{
		auto dirtyComponent = entity_.component<ecs::DirtyComponent>();
		dirtyComponent->dirty |= ecs::DirtyFlags::DIRTY_SOURCE_PATHFINDING | ecs::DirtyFlags::DIRTY_AGENT_STATE;
	}
	else
	{
		entity_.assign<ecs::DirtyComponent>(ecs::DirtyFlags::DIRTY_SOURCE_PATHFINDING | ecs::DirtyFlags::DIRTY_AGENT_STATE);
	}
}


}
