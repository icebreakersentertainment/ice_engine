#ifndef COLLISION_SHAPE_HANDLE_H_
#define COLLISION_SHAPE_HANDLE_H_

#include "physics/ResourceHandle.hpp"

namespace hercules
{
namespace physics
{

class CollisionShapeHandle : public ResourceHandle
{
public:
	using ResourceHandle::ResourceHandle;
	
	static const CollisionShapeHandle INVALID;
};

}
}

#endif /* COLLISION_SHAPE_HANDLE_H_ */
