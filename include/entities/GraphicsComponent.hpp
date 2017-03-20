#ifndef GRAPHICSCOMPONENT_H_
#define GRAPHICSCOMPONENT_H_

#include <entityx/entityx.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

namespace hercules
{
namespace entities
{

struct GraphicsComponent : public entityx::Component<GraphicsComponent>
{
	glm::vec3 position;
	glm::vec3 lookAt;
	glm::vec3 scale;
};

}
}

#endif /* GRAPHICSCOMPONENT_H_ */
