#ifndef ICEENGINEPATHFINDINGAGENTSTATECHANGELISTENER_H_
#define ICEENGINEPATHFINDINGAGENTSTATECHANGELISTENER_H_

#include "pathfinding/IAgentStateChangeListener.hpp"

#include "Scene.hpp"

namespace ice_engine
{

class IceEnginePathfindingAgentStateChangeListener : public pathfinding::IAgentStateChangeListener
{
public:
	IceEnginePathfindingAgentStateChangeListener(ecs::Entity entity, Scene* scene);
	virtual ~IceEnginePathfindingAgentStateChangeListener();
	
	virtual void update(const pathfinding::AgentState& agentState) override;

private:
	ecs::Entity entity_;
	Scene* scene_;
};

}

#endif /* ICEENGINEPATHFINDINGAGENTSTATECHANGELISTENER_H_ */
