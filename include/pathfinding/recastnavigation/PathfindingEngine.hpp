#ifndef PATHFINDINGENGINE_H_
#define PATHFINDINGENGINE_H_

#include <memory>

#include <Recast/Recast.h>
#include <Detour/DetourNavMesh.h>
#include <Detour/DetourNavMeshQuery.h>
#include <DetourCrowd/DetourCrowd.h>

#include "pathfinding/IPathfindingEngine.hpp"

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

struct RecastNavigationPathfindingScene
{
	
};

class PathfindingEngine : public IPathfindingEngine
{
public:
	PathfindingEngine(utilities::Properties* properties, fs::IFileSystem* fileSystem, logger::ILogger* logger);
	virtual ~PathfindingEngine();
	
	virtual void tick(const PathfindingSceneHandle& pathfindingSceneHandle, const float32 delta) override;
	
	virtual PathfindingSceneHandle createPathfindingScene() override;
	virtual void destroyPathfindingScene(const PathfindingSceneHandle& pathfindingSceneHandle) override;
	
	virtual void test(std::vector<glm::vec3>& vertices, std::vector<uint32>& indices) override;
	virtual void test2(entities::Entity entity, IScene* scene, const glm::vec3& position, const glm::quat& orientation) override;
	virtual void test3(entities::Entity entity, const glm::vec3& position, const glm::quat& orientation) override;
	

private:
	PathfindingEngine(const PathfindingEngine& other);
	
	utilities::Properties* properties_;
	fs::IFileSystem* fileSystem_;
	logger::ILogger* logger_;
	
	handles::HandleVector<RecastNavigationPathfindingScene, PathfindingSceneHandle> pathfindingScenes_;
	
	std::vector<entities::Entity> entities_;
	IScene* scene_;
};

}
}
}

#endif /* PATHFINDINGENGINE_H_ */
