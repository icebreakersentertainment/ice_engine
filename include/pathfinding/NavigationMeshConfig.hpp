#ifndef NAVIGATIONMESHCONFIG_H_
#define NAVIGATIONMESHCONFIG_H_

#include "detail/DebugSerializer.hpp"

#include "Types.hpp"

namespace ice_engine
{
namespace pathfinding
{

struct NavigationMeshConfig
{
	float walkableHeight = 0.0f;
	float walkableRadius = 0.0f;
	float walkableClimb = 0.0f;
};

inline std::ostream& operator<<(std::ostream& os, const NavigationMeshConfig& data)
{
    os << "NavigationMeshConfig("
       PRINT_TO_STREAM(data, walkableHeight)
       PRINT_DELIMITER() PRINT_TO_STREAM(data, walkableRadius)
       PRINT_DELIMITER() PRINT_TO_STREAM(data, walkableClimb)
       << ")";

    return os;
}

}
}

#endif /* NAVIGATIONMESHCONFIG_H_ */
