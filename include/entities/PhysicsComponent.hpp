#ifndef PHYSICSCOMPONENT_H_
#define PHYSICSCOMPONENT_H_

#include "physics/CollisionShapeHandle.hpp"

namespace hercules
{
namespace entities
{

struct PhysicsComponent
{
	PhysicsComponent(physics::CollisionShapeHandle collisionShapeHandle = physics::CollisionShapeHandle::INVALID) : collisionShapeHandle(collisionShapeHandle)
	{
	};
	
	physics::CollisionShapeHandle collisionShapeHandle;
};

}
}

#endif /* PHYSICSCOMPONENT_H_ */
