#ifndef RIGIDBODYOBJECTCOMPONENT_H_
#define RIGIDBODYOBJECTCOMPONENT_H_

#include "physics/RigidBodyObjectHandle.hpp"

namespace ice_engine
{
namespace ecs
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
