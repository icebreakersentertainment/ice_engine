#ifndef ICEENGINEPATHFINDINGAGENTSTATECHANGELISTENER_H_
#define ICEENGINEPATHFINDINGAGENTSTATECHANGELISTENER_H_

#include "pathfinding/IAgentStateChangeListener.hpp"

#include "ecs/Entity.hpp"

namespace ice_engine
{

class IceEnginePathfindingAgentStateChangeListener : public pathfinding::IAgentStateChangeListener
{
public:
	IceEnginePathfindingAgentStateChangeListener(ecs::Entity entity);
	virtual ~IceEnginePathfindingAgentStateChangeListener();
	
	virtual void update(const pathfinding::AgentState& agentState) override;

private:
	ecs::Entity entity_;
};

}

#endif /* ICEENGINEPATHFINDINGAGENTSTATECHANGELISTENER_H_ */
