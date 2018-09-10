#ifndef IAGENTSTATECHANGELISTENER_H_
#define IAGENTSTATECHANGELISTENER_H_

#include "AgentState.hpp"

namespace ice_engine
{
namespace pathfinding
{

class IAgentStateChangeListener
{
public:
	virtual ~IAgentStateChangeListener()
	{
	}
	;
	
	virtual void update(const AgentState& agentState) = 0;
};

}
}

#endif /* IAGENTSTATECHANGELISTENER_H_ */
