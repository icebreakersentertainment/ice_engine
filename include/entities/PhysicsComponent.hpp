#ifndef PHYSICSCOMPONENT_H_
#define PHYSICSCOMPONENT_H_

#include "physics/CollisionBodyHandle.hpp"

namespace hercules
{
namespace entities
{

struct PhysicsComponent
{
	PhysicsComponent(physics::CollisionBodyHandle collisionBodyHandle = physics::CollisionBodyHandle::INVALID) : collisionBodyHandle(collisionBodyHandle)
	{
	};
	
	physics::CollisionBodyHandle collisionBodyHandle;
};

}
}

#endif /* PHYSICSCOMPONENT_H_ */
