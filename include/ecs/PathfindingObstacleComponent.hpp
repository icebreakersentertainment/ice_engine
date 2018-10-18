#ifndef PATHFINDINGOBSTACLECOMPONENT_H_
#define PATHFINDINGOBSTACLECOMPONENT_H_

#include "pathfinding/PolygonMeshHandle.hpp"
#include "pathfinding/ObstacleHandle.hpp"

#include "ecs/Serialization.hpp"

namespace ice_engine
{
namespace ecs
{

struct PathfindingObstacleComponent
{
	PathfindingObstacleComponent() = default;
	
	PathfindingObstacleComponent(
		pathfinding::PolygonMeshHandle polygonMeshHandle,
		float32 radius = 0.0f,
		float32 height = 0.0f,
		pathfinding::ObstacleHandle obstacleHandle = pathfinding::ObstacleHandle()
	)
	:
		polygonMeshHandle(polygonMeshHandle),
		radius(radius),
		height(height),
		obstacleHandle(obstacleHandle)
	{
	};

	static uint8 id() { return 12; }

	pathfinding::PolygonMeshHandle polygonMeshHandle;
	float32 radius = 0.0f;
	float32 height = 0.0f;
	pathfinding::ObstacleHandle obstacleHandle;
};

}
}

namespace boost
{
namespace serialization
{

template<class Archive>
void serialize(Archive& ar, ice_engine::ecs::PathfindingObstacleComponent& c, const unsigned int version)
{
	ar & c.polygonMeshHandle & c.radius & c.height & c.obstacleHandle;
}

}
}

#endif /* PATHFINDINGOBSTACLECOMPONENT_H_ */
