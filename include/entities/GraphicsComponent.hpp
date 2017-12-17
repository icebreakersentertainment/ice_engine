#ifndef GRAPHICSCOMPONENT_H_
#define GRAPHICSCOMPONENT_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "graphics/RenderableHandle.hpp"

namespace ice_engine
{
namespace entities
{

struct GraphicsComponent
{
	GraphicsComponent() = default;
	
	GraphicsComponent(
		glm::vec3 scale,
		graphics::RenderableHandle renderableHandle
	)
		: scale(scale), renderableHandle(renderableHandle)
	{
	};
	
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	graphics::RenderableHandle renderableHandle;
};

}
}

#endif /* GRAPHICSCOMPONENT_H_ */
