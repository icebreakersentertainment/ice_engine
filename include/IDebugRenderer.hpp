#ifndef IDEBUGRENDERER_H_
#define IDEBUGRENDERER_H_

#include <vector>
#include <tuple>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "physics/IPhysicsDebugRenderer.hpp"

namespace ice_engine
{

class IDebugRenderer : virtual public physics::IPhysicsDebugRenderer
{
public:
	virtual ~IDebugRenderer()
	{
	}
	;
	
	virtual void pushLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color) = 0;
	virtual void pushLines(const std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& lineData) = 0;
	
	virtual void render() = 0;
};

}

#endif /* IDEBUGRENDERER_H_ */
