#ifndef NAVIGATIONMESHCONFIG_H_
#define NAVIGATIONMESHCONFIG_H_

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

}
}

#endif /* NAVIGATIONMESHCONFIG_H_ */
