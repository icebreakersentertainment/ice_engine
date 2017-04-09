#ifndef PHYSICSCOMPONENT_H_
#define PHYSICSCOMPONENT_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "physics/CollisionShapeHandle.hpp"

#include "Types.hpp"

namespace hercules
{
namespace entities
{

struct PhysicsComponent
{
	PhysicsComponent(
		glm::vec3 position = glm::vec3(),
		glm::quat orientation = glm::quat(),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
		physics::CollisionShapeHandle collisionShapeHandle = physics::CollisionShapeHandle::INVALID
	)
		: position(position), orientation(orientation), collisionShapeHandle(collisionShapeHandle)
	{
	};
	
	glm::vec3 position;
	glm::quat orientation;
	physics::CollisionShapeHandle collisionShapeHandle;
};

}
}

#endif /* PHYSICSCOMPONENT_H_ */
