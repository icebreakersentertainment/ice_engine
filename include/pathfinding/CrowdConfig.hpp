#ifndef CROWDCONFIG_H_
#define CROWDCONFIG_H_

#include "Types.hpp"

namespace ice_engine
{
namespace pathfinding
{

struct CrowdConfig
{
	int maxAgents = 1000;
	float maxAgentRadius = 0.6f;

};

}
}

#endif /* CROWDCONFIG_H_ */
