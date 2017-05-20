#ifndef COLLISION_BODY_HANDLE_H_
#define COLLISION_BODY_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace physics
{

class CollisionBodyHandle : public handles::Handle
{
public:
	using handles::Handle::Handle;
	
	static const CollisionBodyHandle INVALID;
};

}
}

#endif /* COLLISION_BODY_HANDLE_H_ */
