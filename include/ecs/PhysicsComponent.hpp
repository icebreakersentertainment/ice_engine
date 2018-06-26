#ifndef PHYSICSCOMPONENT_H_
#define PHYSICSCOMPONENT_H_

#include "physics/RigidBodyObjectHandle.hpp"

namespace ice_engine
{
namespace ecs
{

struct RigidBodyObjectComponent
{
	RigidBodyObjectComponent(physics::RigidBodyObjectHandle rigidBodyObjectHandle = physics::RigidBodyObjectHandle::INVALID) : rigidBodyObjectHandle(rigidBodyObjectHandle)
	{
	};
	
	physics::RigidBodyObjectHandle rigidBodyObjectHandle;
};

}
}

#endif /* PHYSICSCOMPONENT_H_ */
