#ifndef CROWDCONFIG_H_
#define CROWDCONFIG_H_

#include "detail/DebugSerializer.hpp"

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

inline std::ostream& operator<<(std::ostream& os, const CrowdConfig& data)
{
    os << "CrowdConfig("
       PRINT_TO_STREAM(data, maxAgents)
       PRINT_DELIMITER() PRINT_TO_STREAM(data, maxAgentRadius)
       << ")";

    return os;
}

}
}

#endif /* CROWDCONFIG_H_ */
