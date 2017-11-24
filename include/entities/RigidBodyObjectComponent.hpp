#ifndef RIGIDBODYOBJECTCOMPONENT_H_
#define RIGIDBODYOBJECTCOMPONENT_H_

#include "physics/RigidBodyObjectHandle.hpp"

namespace hercules
{
namespace entities
{

struct RigidBodyObjectComponent
{
	RigidBodyObjectComponent() = default;
	
	RigidBodyObjectComponent(physics::RigidBodyObjectHandle rigidBodyObjectHandle) : rigidBodyObjectHandle(rigidBodyObjectHandle)
	{
	};
	
	physics::RigidBodyObjectHandle rigidBodyObjectHandle;
};

}
}

#endif /* RIGIDBODYOBJECTCOMPONENT_H_ */
