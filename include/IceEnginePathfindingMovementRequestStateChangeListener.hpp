#ifndef ICEENGINEPATHFINDINGMOVEMENTREQUESTSTATELISTENER_H_
#define ICEENGINEPATHFINDINGMOVEMENTREQUESTSTATELISTENER_H_

#include "pathfinding/IMovementRequestStateChangeListener.hpp"

#include "Scene.hpp"

namespace ice_engine
{

class IceEnginePathfindingMovementRequestStateChangeListener : public pathfinding::IMovementRequestStateChangeListener
{
public:
	IceEnginePathfindingMovementRequestStateChangeListener(entityx::Entity entity, Scene* scene);
	virtual ~IceEnginePathfindingMovementRequestStateChangeListener();
	
	virtual void update(const pathfinding::MovementRequestState& movementRequestState) override;

private:
	entityx::Entity entity_;
	Scene* scene_;
};

}

#endif /* ICEENGINEPATHFINDINGMOVEMENTREQUESTSTATELISTENER_H_ */
