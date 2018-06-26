#ifndef IPATHFINDINGENGINE_H_
#define IPATHFINDINGENGINE_H_

#include <vector>
#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Types.hpp"

#include "pathfinding/UserData.hpp"

#include "pathfinding/PathfindingSceneHandle.hpp"
#include "pathfinding/PolygonMeshHandle.hpp"
#include "pathfinding/NavigationMeshHandle.hpp"
#include "pathfinding/AgentHandle.hpp"
#include "pathfinding/CrowdHandle.hpp"

#include "pathfinding/IAgentMotionChangeListener.hpp"
#include "pathfinding/IMovementRequestStateChangeListener.hpp"

#include "pathfinding/IPathfindingDebugRenderer.hpp"

namespace ice_engine
{
namespace pathfinding
{

struct PolygonMeshConfig
{
	int width = 0;
	int height = 0;
	int tileSize = 0;
	int borderSize = 0;

	float cellSize = 1.0f;
	float cellHeight = 1.0f;

	float walkableSlopeAngle = 0.0f;
	int walkableHeight = 0;
	int walkableClimb = 0;
	int walkableRadius = 0;
	int maxEdgeLength = 0;

	float maxSimplificationError = 0.0f;

	int minRegionArea = 0;
	int mergeRegionArea = 0;
	int maxVertsPerPoly = 3;

	float detailSampleDist = 0.0f;

	float detailSampleMaxError = 0.0f;
};

struct NavigationMeshConfig
{
	float walkableHeight = 0.0f;
	float walkableRadius = 0.0f;
	float walkableClimb = 0.0f;
};

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

class IPathfindingEngine
{
public:
	virtual ~IPathfindingEngine()
	{
	}
	;
	
	virtual void tick(const PathfindingSceneHandle& pathfindingSceneHandle, const float32 delta) = 0;
	
	virtual PathfindingSceneHandle createPathfindingScene() = 0;
	virtual void destroyPathfindingScene(const PathfindingSceneHandle& pathfindingSceneHandle) = 0;
	
	virtual void setPathfindingDebugRenderer(IPathfindingDebugRenderer* pathfindingDebugRenderer) = 0;
	virtual void setDebugRendering(const PathfindingSceneHandle& pathfindingSceneHandle, const bool enabled) = 0;
	
	virtual PolygonMeshHandle createPolygonMesh(const std::vector<glm::vec3>& vertices, const std::vector<uint32>& indices, const PolygonMeshConfig& polygonMeshConfig = PolygonMeshConfig()) = 0;
	virtual NavigationMeshHandle createNavigationMesh(const PolygonMeshHandle& polygonMeshHandle, const NavigationMeshConfig& navigationMeshConfig = NavigationMeshConfig()) = 0;
	
	virtual CrowdHandle createCrowd(const PathfindingSceneHandle& pathfindingSceneHandle, const NavigationMeshHandle& navigationMeshHandle) = 0;
	virtual void destroy(const PathfindingSceneHandle& pathfindingSceneHandle, const CrowdHandle& crowdHandle) = 0;
	
	virtual AgentHandle createAgent(
		const PathfindingSceneHandle& pathfindingSceneHandle,
		const CrowdHandle& crowdHandle,
		const glm::vec3& position,
		const AgentParams& agentParams = AgentParams(),
		std::unique_ptr<IAgentMotionChangeListener> agentMotionChangeListener = nullptr,
		std::unique_ptr<IMovementRequestStateChangeListener> movementRequestStateChangeListener = nullptr,
		const UserData& userData = UserData()
	) = 0;
	virtual void destroy(const PathfindingSceneHandle& pathfindingSceneHandle, const CrowdHandle& crowdHandle, const AgentHandle& agentHandle) = 0;
	
	virtual void requestMoveTarget(
		const PathfindingSceneHandle& pathfindingSceneHandle,
		const CrowdHandle& crowdHandle,
		const AgentHandle& agentHandle,
		const glm::vec3& position
	) = 0;
	
	virtual void requestMoveVelocity(
		const PathfindingSceneHandle& pathfindingSceneHandle,
		const CrowdHandle& crowdHandle,
		const AgentHandle& agentHandle,
		const glm::vec3& velocity
	) = 0;
	
	//virtual void test2(entityx::Entity entity, Scene* scene, const glm::vec3& position, const glm::quat& orientation) = 0;
	//virtual void test3(entityx::Entity entity, const glm::vec3& position, const glm::quat& orientation) = 0;
	
	virtual void setMotionChangeListener(
		const PathfindingSceneHandle& pathfindingSceneHandle,
		const CrowdHandle& crowdHandle,
		const AgentHandle& agentHandle,
		std::unique_ptr<IAgentMotionChangeListener> agentMotionChangeListener
	) = 0;
	virtual void setStateChangeListener(
		const PathfindingSceneHandle& pathfindingSceneHandle,
		const CrowdHandle& crowdHandle,
		const AgentHandle& agentHandle,
		std::unique_ptr<IMovementRequestStateChangeListener> movementRequestStateChangeListener
	) = 0;
	
	virtual void setUserData(
		const PathfindingSceneHandle& pathfindingSceneHandle,
		const CrowdHandle& crowdHandle,
		const AgentHandle& agentHandle,
		const UserData& userData
	) = 0;
	virtual UserData& getUserData(
		const PathfindingSceneHandle& pathfindingSceneHandle,
		const CrowdHandle& crowdHandle,
		const AgentHandle& agentHandle
	) const = 0;
};

}
}

#endif /* IPATHFINDINGENGINE_H_ */
