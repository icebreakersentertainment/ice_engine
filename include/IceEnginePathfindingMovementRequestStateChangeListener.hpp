#ifndef ICEENGINEPATHFINDINGMOVEMENTREQUESTSTATECHANGELISTENER_H_
#define ICEENGINEPATHFINDINGMOVEMENTREQUESTSTATECHANGELISTENER_H_

#include "pathfinding/IMovementRequestStateChangeListener.hpp"

#include "ecs/Entity.hpp"

namespace ice_engine
{

class IceEnginePathfindingMovementRequestStateChangeListener : public pathfinding::IMovementRequestStateChangeListener
{
public:
	IceEnginePathfindingMovementRequestStateChangeListener(ecs::Entity entity);
	virtual ~IceEnginePathfindingMovementRequestStateChangeListener();
	
	virtual void update(const pathfinding::MovementRequestState& movementRequestState) override;

private:
	ecs::Entity entity_;
};

}

#endif /* ICEENGINEPATHFINDINGMOVEMENTREQUESTSTATECHANGELISTENER_H_ */
