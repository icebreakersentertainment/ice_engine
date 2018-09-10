#ifndef PATHFINDINGENGINE_H_
#define PATHFINDINGENGINE_H_

#include <memory>

#include <Recast/Recast.h>
#include <Detour/DetourNavMesh.h>
#include <Detour/DetourNavMeshBuilder.h>
#include <Detour/DetourNavMeshQuery.h>
#include <DetourCrowd/DetourCrowd.h>

#include "pathfinding/IPathfindingEngine.hpp"

#include "DebugRenderer.hpp"

#include "handles/HandleVector.hpp"
#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace ice_engine
{
namespace pathfinding
{
namespace recastnavigation
{

struct RecastNavigationPolygonMesh
{
	std::unique_ptr<rcHeightfield, decltype(&rcFreeHeightField)> heightField = std::unique_ptr<rcHeightfield, decltype(&rcFreeHeightField)>(nullptr, rcFreeHeightField);
	std::unique_ptr<rcCompactHeightfield, decltype(&rcFreeCompactHeightfield)> compactHeightfield = std::unique_ptr<rcCompactHeightfield, decltype(&rcFreeCompactHeightfield)>(nullptr, rcFreeCompactHeightfield);
	std::unique_ptr<rcContourSet, decltype(&rcFreeContourSet)> contourSet = std::unique_ptr<rcContourSet, decltype(&rcFreeContourSet)>(nullptr, rcFreeContourSet);
	std::unique_ptr<rcPolyMesh, decltype(&rcFreePolyMesh)> polyMesh = std::unique_ptr<rcPolyMesh, decltype(&rcFreePolyMesh)>(nullptr, rcFreePolyMesh);
	std::unique_ptr<rcPolyMeshDetail, decltype(&rcFreePolyMeshDetail)> polyMeshDetail = std::unique_ptr<rcPolyMeshDetail, decltype(&rcFreePolyMeshDetail)>(nullptr, rcFreePolyMeshDetail);
	
	rcConfig config;
};

struct RecastNavigationNavigationMesh
{
	PolygonMeshHandle polygonMeshHandle;

	std::unique_ptr<dtNavMesh, decltype(&dtFreeNavMesh)> navMesh = std::unique_ptr<dtNavMesh, decltype(&dtFreeNavMesh)>(nullptr, dtFreeNavMesh);
	std::unique_ptr<dtNavMeshQuery, decltype(&dtFreeNavMeshQuery)> navMeshQuery = std::unique_ptr<dtNavMeshQuery, decltype(&dtFreeNavMeshQuery)>(nullptr, dtFreeNavMeshQuery);
	
	dtNavMeshCreateParams params;
};

struct RecastNavigationAgent
{
	int32 index = -1;
	unsigned char state = DT_CROWDAGENT_STATE_INVALID;
	unsigned char moveRequestState = DT_CROWDAGENT_TARGET_NONE;
	std::unique_ptr<IAgentMotionChangeListener> agentMotionChangeListener = nullptr;
	std::unique_ptr<IAgentStateChangeListener> agentStateChangeListener = nullptr;
	std::unique_ptr<IMovementRequestStateChangeListener> movementRequestStateChangeListener = nullptr;
};

struct RecastNavigationCrowd
{
	NavigationMeshHandle navigationMeshHandle;
	std::unique_ptr<dtCrowd, decltype(&dtFreeCrowd)> crowd = std::unique_ptr<dtCrowd, decltype(&dtFreeCrowd)>(nullptr, dtFreeCrowd);
	
	handles::HandleVector<RecastNavigationAgent, AgentHandle> agents;
};

struct RecastNavigationPathfindingScene
{
	handles::HandleVector<RecastNavigationCrowd, CrowdHandle> crowds;

	bool debugRendering = false;
};

class PathfindingEngine : public IPathfindingEngine
{
public:
	PathfindingEngine(utilities::Properties* properties, fs::IFileSystem* fileSystem, logger::ILogger* logger);
	virtual ~PathfindingEngine();
	
	virtual void tick(const PathfindingSceneHandle& pathfindingSceneHandle, const float32 delta) override;
	
	virtual PathfindingSceneHandle createPathfindingScene() override;
	virtual void destroyPathfindingScene(const PathfindingSceneHandle& pathfindingSceneHandle) override;
	
	virtual void setPathfindingDebugRenderer(IPathfindingDebugRenderer* pathfindingDebugRenderer) override;
	virtual void setDebugRendering(const PathfindingSceneHandle& pathfindingSceneHandle, const bool enabled) override;
	
	virtual PolygonMeshHandle createPolygonMesh(const ITerrain* terrain, const PolygonMeshConfig& polygonMeshConfig = PolygonMeshConfig()) override;
	virtual void destroy(const PolygonMeshHandle& polygonMeshHandle) override;

	virtual NavigationMeshHandle createNavigationMesh(const PolygonMeshHandle& polygonMeshHandle, const NavigationMeshConfig& navigationMeshConfig = NavigationMeshConfig()) override;
	virtual void destroy(const NavigationMeshHandle& navigationMeshHandle) override;
	
	virtual CrowdHandle createCrowd(const PathfindingSceneHandle& pathfindingSceneHandle, const NavigationMeshHandle& navigationMeshHandle) override;
	virtual void destroy(const PathfindingSceneHandle& pathfindingSceneHandle, const CrowdHandle& crowdHandle) override;
	
	virtual AgentHandle createAgent(
		const PathfindingSceneHandle& pathfindingSceneHandle,
		const CrowdHandle& crowdHandle,
		const glm::vec3& position,
		const AgentParams& agentParams = AgentParams(),
		std::unique_ptr<IAgentMotionChangeListener> agentMotionChangeListener = nullptr,
		std::unique_ptr<IAgentStateChangeListener> agentStateChangeListener = nullptr,
		std::unique_ptr<IMovementRequestStateChangeListener> movementRequestStateChangeListener = nullptr,
		const UserData& userData = UserData()
	) override;
	virtual void destroy(const PathfindingSceneHandle& pathfindingSceneHandle, const CrowdHandle& crowdHandle, const AgentHandle& agentHandle) override;
	
	virtual void requestMoveTarget(
		const PathfindingSceneHandle& pathfindingSceneHandle,
		const CrowdHandle& crowdHandle,
		const AgentHandle& agentHandle,
		const glm::vec3& position
	) override;
	
	virtual void requestMoveVelocity(
		const PathfindingSceneHandle& pathfindingSceneHandle,
		const CrowdHandle& crowdHandle,
		const AgentHandle& agentHandle,
		const glm::vec3& velocity
	) override;
	
	//virtual void test2(entityx::Entity entity, Scene* scene, const glm::vec3& position, const glm::quat& orientation) override;
	//virtual void test3(entityx::Entity entity, const glm::vec3& position, const glm::quat& orientation) override;
	
	virtual void setMotionChangeListener(
		const PathfindingSceneHandle& pathfindingSceneHandle,
		const CrowdHandle& crowdHandle,
		const AgentHandle& agentHandle,
		std::unique_ptr<IAgentMotionChangeListener> agentMotionChangeListener
	) override;
	virtual void setStateChangeListener(
		const PathfindingSceneHandle& pathfindingSceneHandle,
		const CrowdHandle& crowdHandle,
		const AgentHandle& agentHandle,
		std::unique_ptr<IAgentStateChangeListener> agentStateChangeListener
	) override;
	virtual void setMovementRequestChangeListener(
		const PathfindingSceneHandle& pathfindingSceneHandle,
		const CrowdHandle& crowdHandle,
		const AgentHandle& agentHandle,
		std::unique_ptr<IMovementRequestStateChangeListener> movementRequestStateChangeListener
	) override;
	
	virtual void setUserData(
		const PathfindingSceneHandle& pathfindingSceneHandle,
		const CrowdHandle& crowdHandle,
		const AgentHandle& agentHandle,
		const UserData& userData
	) override;
	virtual UserData& getUserData(
		const PathfindingSceneHandle& pathfindingSceneHandle,
		const CrowdHandle& crowdHandle,
		const AgentHandle& agentHandle
	) const override;
	

private:
	PathfindingEngine(const PathfindingEngine& other);
	
	utilities::Properties* properties_;
	fs::IFileSystem* fileSystem_;
	logger::ILogger* logger_;
	
	handles::HandleVector<RecastNavigationPathfindingScene, PathfindingSceneHandle> pathfindingScenes_;
	handles::HandleVector<RecastNavigationPolygonMesh, PolygonMeshHandle> polygonMeshes_;
	handles::HandleVector<RecastNavigationNavigationMesh, NavigationMeshHandle> navigationMeshes_;
	
	std::unique_ptr<DebugRenderer> debugRenderer_;
};

}
}
}

#endif /* PATHFINDINGENGINE_H_ */
