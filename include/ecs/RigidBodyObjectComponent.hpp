#ifndef RIGIDBODYOBJECTCOMPONENT_H_
#define RIGIDBODYOBJECTCOMPONENT_H_

#include "physics/CollisionShapeHandle.hpp"
#include "physics/RigidBodyObjectHandle.hpp"

#include "serialization/Serialization.hpp"

namespace ice_engine
{
namespace ecs
{

struct RigidBodyObjectComponent
{
	RigidBodyObjectComponent() = default;
	
	RigidBodyObjectComponent(physics::CollisionShapeHandle collisionShapeHandle) : collisionShapeHandle(collisionShapeHandle)
	{
	};

	RigidBodyObjectComponent(physics::CollisionShapeHandle collisionShapeHandle, float32 mass, float32 friction, float32 restitution)
		:
		collisionShapeHandle(collisionShapeHandle),
		mass(mass),
		friction(friction),
		restitution(restitution)
	{
	};

	RigidBodyObjectComponent(physics::CollisionShapeHandle collisionShapeHandle, float32 mass, float32 friction, float32 restitution, physics::RigidBodyObjectHandle rigidBodyObjectHandle)
		:
		collisionShapeHandle(collisionShapeHandle),
		mass(mass),
		friction(friction),
		restitution(restitution),
		rigidBodyObjectHandle(rigidBodyObjectHandle)
	{
	};
	
	static uint8 id()  { return 4; }

	physics::CollisionShapeHandle collisionShapeHandle;
	float32 mass = 1.0f;
	float32 friction = 1.0f;
	float32 restitution = 1.0f;
	physics::RigidBodyObjectHandle rigidBodyObjectHandle;
};

}
}

namespace boost
{
namespace serialization
{

template<class Archive>
void serialize(Archive& ar, ice_engine::ecs::RigidBodyObjectComponent& c, const unsigned int version)
{
	ar & c.collisionShapeHandle & c.mass & c.friction & c.restitution & c.rigidBodyObjectHandle;
}

}
}

#endif /* RIGIDBODYOBJECTCOMPONENT_H_ */
