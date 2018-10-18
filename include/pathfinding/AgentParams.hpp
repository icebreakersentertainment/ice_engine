#ifndef AGENTPARAMS_H_
#define AGENTPARAMS_H_

#include "detail/DebugSerializer.hpp"

namespace ice_engine
{
namespace pathfinding
{

struct AgentParams
{
	float radius = 0.0f;
	float height = 0.1f;						///< Agent height. [Limit: > 0]
	float maxAcceleration = 0.0f;
	float maxSpeed = 0.0f;

	/// Defines how close a collision element must be before it is considered for steering behaviors. [Limits: > 0]
	float collisionQueryRange = 0.1f;

	float pathOptimizationRange = 0.1f;		///< The path visibility optimization range. [Limit: > 0]

	float separationWeight = 0.0f;

	/// Flags that impact steering behavior. (See: #UpdateFlags)
	//unsigned char updateFlags;

	/// The index of the avoidance configuration to use for the agent.
	/// [Limits: 0 <= value <= #DT_CROWD_MAX_OBSTAVOIDANCE_PARAMS]
	//unsigned char obstacleAvoidanceType;

	/// The index of the query filter used by this agent.
	//unsigned char queryFilterType;

	/// User defined data attached to the agent.
	//void* userData;
};

inline std::ostream& operator<<(std::ostream& os, const AgentParams& data)
{
    os << "AgentParams("
		PRINT_TO_STREAM(data, radius)
		PRINT_DELIMITER() PRINT_TO_STREAM(data, height)
		PRINT_DELIMITER() PRINT_TO_STREAM(data, maxAcceleration)
		PRINT_DELIMITER() PRINT_TO_STREAM(data, maxSpeed)
		PRINT_DELIMITER() PRINT_TO_STREAM(data, collisionQueryRange)
		PRINT_DELIMITER() PRINT_TO_STREAM(data, pathOptimizationRange)
		PRINT_DELIMITER() PRINT_TO_STREAM(data, separationWeight)
		<< ")";

    return os;
}

}
}

#endif /* AGENTPARAMS_H_ */
