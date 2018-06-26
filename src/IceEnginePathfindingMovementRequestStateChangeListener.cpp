#include "IceEnginePathfindingMovementRequestStateChangeListener.hpp"

namespace ice_engine
{

IceEnginePathfindingMovementRequestStateChangeListener::IceEnginePathfindingMovementRequestStateChangeListener(entityx::Entity entity, Scene* scene) : entity_(entity), scene_(scene)
{
	
}

IceEnginePathfindingMovementRequestStateChangeListener::~IceEnginePathfindingMovementRequestStateChangeListener()
{
}

void IceEnginePathfindingMovementRequestStateChangeListener::update(const pathfinding::MovementRequestState& movementRequestState)
{
	//scene_->position(entity_, position);
}


}
