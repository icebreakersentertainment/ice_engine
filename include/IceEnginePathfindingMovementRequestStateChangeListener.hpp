#ifndef ICEENGINEPATHFINDINGMOVEMENTREQUESTSTATECHANGELISTENER_H_
#define ICEENGINEPATHFINDINGMOVEMENTREQUESTSTATECHANGELISTENER_H_

#include "pathfinding/IMovementRequestStateChangeListener.hpp"

#include "Scene.hpp"

namespace ice_engine
{

class IceEnginePathfindingMovementRequestStateChangeListener : public pathfinding::IMovementRequestStateChangeListener
{
public:
	IceEnginePathfindingMovementRequestStateChangeListener(ecs::Entity entity, Scene* scene);
	virtual ~IceEnginePathfindingMovementRequestStateChangeListener();
	
	virtual void update(const pathfinding::MovementRequestState& movementRequestState) override;

private:
	ecs::Entity entity_;
	Scene* scene_;
};

}

#endif /* ICEENGINEPATHFINDINGMOVEMENTREQUESTSTATECHANGELISTENER_H_ */
