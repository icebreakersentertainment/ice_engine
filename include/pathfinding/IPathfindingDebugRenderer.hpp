#ifndef IPATHFINDINGDEBUGRENDERER_H_
#define IPATHFINDINGDEBUGRENDERER_H_

#include <vector>
#include <tuple>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace ice_engine
{
namespace pathfinding
{

class IPathfindingDebugRenderer
{
public:
	virtual ~IPathfindingDebugRenderer() = default;
	
	virtual void pushLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color) = 0;
	virtual void pushLines(const std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& lineData) = 0;
};

}
}

#endif /* IPATHFINDINGDEBUGRENDERER_H_ */
