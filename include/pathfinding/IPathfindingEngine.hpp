#ifndef IPATHFINDINGENGINE_H_
#define IPATHFINDINGENGINE_H_

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Types.hpp"

#include "entities/Entity.hpp"
#include "IScene.hpp"

#include "pathfinding/PathfindingSceneHandle.hpp"

namespace hercules
{
namespace pathfinding
{

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
	
	virtual void test(std::vector<glm::vec3>& vertices, std::vector<uint32>& indices) = 0;
	virtual void test2(entities::Entity entity, IScene* scene, const glm::vec3& position, const glm::quat& orientation) = 0;
	virtual void test3(entities::Entity entity, const glm::vec3& position, const glm::quat& orientation) = 0;
};

}
}

#endif /* IPATHFINDINGENGINE_H_ */
