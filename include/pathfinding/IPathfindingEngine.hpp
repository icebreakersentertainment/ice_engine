#ifndef IPATHFINDINGENGINE_H_
#define IPATHFINDINGENGINE_H_

#include <vector>
#include <memory>

#include <boost/any.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Types.hpp"

#include "pathfinding/UserData.hpp"

#include "pathfinding/PathfindingSceneHandle.hpp"
#include "pathfinding/PolygonMeshHandle.hpp"
#include "pathfinding/NavigationMeshHandle.hpp"
#include "pathfinding/AgentHandle.hpp"
#include "pathfinding/ObstacleHandle.hpp"
#include "pathfinding/CrowdHandle.hpp"
#include "pathfinding/PolygonMeshConfig.hpp"
#include "pathfinding/NavigationMeshConfig.hpp"
#include "pathfinding/CrowdConfig.hpp"
#include "pathfinding/AgentParams.hpp"
#include "pathfinding/AgentState.hpp"
#include "pathfinding/MovementRequestState.hpp"

#include "pathfinding/IAgentMotionChangeListener.hpp"
#include "pathfinding/IAgentStateChangeListener.hpp"
#include "pathfinding/IMovementRequestStateChangeListener.hpp"

#include "pathfinding/IPathfindingDebugRenderer.hpp"

#include "pathfinding/ITerrain.hpp"

namespace ice_engine
{
namespace pathfinding
{

class IPathfindingEngine
{
public:
	virtual ~IPathfindingEngine() = default;
	
	virtual void tick(const PathfindingSceneHandle& pathfindingSceneHandle, const float32 delta) = 0;
    virtual void renderDebug(const PathfindingSceneHandle& pathfindingSceneHandle) = 0;
	
	virtual PathfindingSceneHandle createPathfindingScene() = 0;
	virtual void destroyPathfindingScene(const PathfindingSceneHandle& pathfindingSceneHandle) = 0;
	
	virtual void setPathfindingDebugRenderer(IPathfindingDebugRenderer* pathfindingDebugRenderer) = 0;
	virtual void setDebugRendering(const PathfindingSceneHandle& pathfindingSceneHandle, const bool enabled) = 0;
	
	virtual PolygonMeshHandle createPolygonMesh(const ITerrain* terrain, const PolygonMeshConfig& polygonMeshConfig = PolygonMeshConfig()) = 0;
	virtual void destroy(const PolygonMeshHandle& polygonMeshHandle) = 0;

	virtual ObstacleHandle createObstacle(const PolygonMeshHandle& polygonMeshHandle, const glm::vec3& position, const float32 radius, const float32 height) = 0;
	virtual void destroy(const PolygonMeshHandle& polygonMeshHandle, const ObstacleHandle& obstacleHandle) = 0;

	virtual NavigationMeshHandle createNavigationMesh(const PolygonMeshHandle& polygonMeshHandle, const NavigationMeshConfig& navigationMeshConfig = NavigationMeshConfig()) = 0;
	virtual void destroy(const NavigationMeshHandle& navigationMeshHandle) = 0;
	
	virtual CrowdHandle createCrowd(const PathfindingSceneHandle& pathfindingSceneHandle, const NavigationMeshHandle& navigationMeshHandle, const CrowdConfig& crowdConfig) = 0;
	virtual void destroy(const PathfindingSceneHandle& pathfindingSceneHandle, const CrowdHandle& crowdHandle) = 0;
	
	virtual AgentHandle createAgent(
            const PathfindingSceneHandle& pathfindingSceneHandle,
            const CrowdHandle& crowdHandle,
            const glm::vec3& position,
            const AgentParams& agentParams = AgentParams(),
            std::unique_ptr<IAgentMotionChangeListener> agentMotionChangeListener = nullptr,
            std::unique_ptr<IAgentStateChangeListener> agentStateChangeListener = nullptr,
            std::unique_ptr<IMovementRequestStateChangeListener> movementRequestStateChangeListener = nullptr,
            const boost::any& userData = UserData()
	) = 0;
	virtual void destroy(const PathfindingSceneHandle& pathfindingSceneHandle, const CrowdHandle& crowdHandle, const AgentHandle& agentHandle) = 0;
	
//	virtual ObstacleHandle createBoxObstacle(
//		const PolygonMeshHandle& polygonMeshHandle,
//		const glm::vec3& position,
//		const glm::vec3& dimensions,
//		const float32 yRadians = 0.0f
//	) = 0;
//	virtual void destroy(const PolygonMeshHandle& polygonMeshHandle, const ObstacleHandle& obstacleHandle) = 0;

	virtual void requestMoveTarget(
		const PathfindingSceneHandle& pathfindingSceneHandle,
		const CrowdHandle& crowdHandle,
		const AgentHandle& agentHandle,
		const glm::vec3& position
	) = 0;

	virtual void resetMoveTarget(
		const PathfindingSceneHandle& pathfindingSceneHandle,
		const CrowdHandle& crowdHandle,
		const AgentHandle& agentHandle
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
		std::unique_ptr<IAgentStateChangeListener> agentStateChangeListener
	) = 0;
	virtual void setMovementRequestChangeListener(
		const PathfindingSceneHandle& pathfindingSceneHandle,
		const CrowdHandle& crowdHandle,
		const AgentHandle& agentHandle,
		std::unique_ptr<IMovementRequestStateChangeListener> movementRequestStateChangeListener
	) = 0;
	
	virtual void setUserData(
        const PathfindingSceneHandle& pathfindingSceneHandle,
        const CrowdHandle& crowdHandle,
        const AgentHandle& agentHandle,
        const boost::any& userData
	) = 0;
	virtual boost::any& getUserData(
		const PathfindingSceneHandle& pathfindingSceneHandle,
		const CrowdHandle& crowdHandle,
		const AgentHandle& agentHandle
	) const = 0;
};

}
}

#endif /* IPATHFINDINGENGINE_H_ */
