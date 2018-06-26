#ifndef AGENT_HANDLE_H_
#define AGENT_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace pathfinding
{

class AgentHandle : public handles::Handle<AgentHandle>
{
public:
	using handles::Handle<AgentHandle>::Handle;
};

}
}

#endif /* AGENT_HANDLE_H_ */
