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

namespace std
{
	template <>
	struct hash<ice_engine::pathfinding::AgentHandle>
	{
		std::size_t operator()(const ice_engine::pathfinding::AgentHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* AGENT_HANDLE_H_ */
