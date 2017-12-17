#ifndef IPHYSICSDEBUGRENDERER_H_
#define IPHYSICSDEBUGRENDERER_H_

#include <vector>
#include <tuple>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace ice_engine
{
namespace physics
{

class IPhysicsDebugRenderer
{
public:
	virtual ~IPhysicsDebugRenderer()
	{
	}
	
	virtual void pushLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color) = 0;
	virtual void pushLines(const std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& lineData) = 0;
};

}
}

#endif /* IPHYSICSDEBUGRENDERER_H_ */
