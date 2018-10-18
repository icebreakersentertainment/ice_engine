#ifndef GHOSTOBJECTCOMPONENT_H_
#define GHOSTOBJECTCOMPONENT_H_

#include "physics/GhostObjectHandle.hpp"

namespace ice_engine
{
namespace ecs
{

struct GhostObjectComponent
{
	GhostObjectComponent() = default;
	
	GhostObjectComponent(physics::CollisionShapeHandle collisionShapeHandle) : collisionShapeHandle(collisionShapeHandle)
	{
	};

	GhostObjectComponent(physics::CollisionShapeHandle collisionShapeHandle, physics::GhostObjectHandle ghostObjectHandle) : collisionShapeHandle(collisionShapeHandle), ghostObjectHandle(ghostObjectHandle)
	{
	};
	
	static uint8 id()  { return 5; }

	physics::CollisionShapeHandle collisionShapeHandle;
	physics::GhostObjectHandle ghostObjectHandle;
};

}
}

namespace boost
{
namespace serialization
{

template<class Archive>
void serialize(Archive& ar, ice_engine::ecs::GhostObjectComponent& c, const unsigned int version)
{
	ar & c.collisionShapeHandle & c.ghostObjectHandle;
}

}
}

#endif /* GHOSTOBJECTCOMPONENT_H_ */
