#include "IceEnginePathfindingMovementRequestStateChangeListener.hpp"

namespace ice_engine
{

IceEnginePathfindingMovementRequestStateChangeListener::IceEnginePathfindingMovementRequestStateChangeListener(ecs::Entity entity) : entity_(entity)
{
	
}

IceEnginePathfindingMovementRequestStateChangeListener::~IceEnginePathfindingMovementRequestStateChangeListener()
{
}

void IceEnginePathfindingMovementRequestStateChangeListener::update(const pathfinding::MovementRequestState& movementRequestState)
{
	auto pac = entity_.component<ecs::PathfindingAgentComponent>();

	pac->movementRequestState = movementRequestState;

	if (entity_.hasComponent<ecs::DirtyComponent>())
	{
		auto dirtyComponent = entity_.component<ecs::DirtyComponent>();
		dirtyComponent->dirty |= ecs::DirtyFlags::DIRTY_SOURCE_PATHFINDING | ecs::DirtyFlags::DIRTY_MOVEMENT_REQUEST_STATE;
	}
	else
	{
		entity_.assign<ecs::DirtyComponent>(ecs::DirtyFlags::DIRTY_SOURCE_PATHFINDING | ecs::DirtyFlags::DIRTY_MOVEMENT_REQUEST_STATE);
	}
}


}
