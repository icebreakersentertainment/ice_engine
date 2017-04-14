#ifndef COLLISION_BODY_HANDLE_H_
#define COLLISION_BODY_HANDLE_H_

#include "physics/ResourceHandle.hpp"

namespace hercules
{
namespace physics
{

class CollisionBodyHandle : public ResourceHandle
{
public:
	using ResourceHandle::ResourceHandle;
	
	static const CollisionBodyHandle INVALID;
};

}
}

#endif /* COLLISION_BODY_HANDLE_H_ */
