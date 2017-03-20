#ifndef GRAPHICSCOMPONENT_H_
#define GRAPHICSCOMPONENT_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "graphics/ModelHandle.hpp"

#include "Types.hpp"

namespace hercules
{
namespace entities
{

struct GraphicsComponent
{
	GraphicsComponent(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 lookAt = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
		graphics::ModelHandle modelHandle = graphics::ModelHandle::INVALID
	)
		: position(position), scale(scale), modelHandle(modelHandle)
	{
		const glm::mat4 lookAtMatrix = glm::lookAt(position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
		orientation =  glm::normalize( orientation * glm::quat_cast( lookAtMatrix ) );
	};
	
	GraphicsComponent(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat orientation = glm::quat(),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
		graphics::ModelHandle modelHandle = graphics::ModelHandle::INVALID
	)
		: position(position), orientation(orientation), scale(scale), modelHandle(modelHandle)
	{
	};
	
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat orientation;
	graphics::ModelHandle modelHandle;
};

}
}

#endif /* GRAPHICSCOMPONENT_H_ */
