#ifndef POSITIONORIENTATIONCOMPONENT_H_
#define POSITIONORIENTATIONCOMPONENT_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ice_engine
{
namespace ecs
{

struct PositionOrientationComponent
{
	PositionOrientationComponent() = default;
	
	PositionOrientationComponent(glm::vec3 position, glm::quat orientation) : position(position), orientation(orientation)
	{
	};
	
	PositionOrientationComponent(glm::vec3 position, glm::vec3 lookAt) : position(position)
	{
		const glm::mat4 lookAtMatrix = glm::lookAt(position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
		orientation =  glm::normalize( orientation * glm::quat_cast( lookAtMatrix ) );
	};
	
	glm::vec3 position;
	glm::quat orientation;
};

}
}

#endif /* POSITIONORIENTATIONCOMPONENT_H_ */
