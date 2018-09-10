#ifndef AGENTSTATE_H_
#define AGENTSTATE_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

namespace ice_engine
{
namespace pathfinding
{

enum AgentState : uint32
{
	INVALID = 0,
	WALKING,
	OFFMESH,
};

}
}

#endif /* AGENTSTATE_H_ */
