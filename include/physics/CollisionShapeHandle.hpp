#ifndef COLLISION_SHAPE_HANDLE_H_
#define COLLISION_SHAPE_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace physics
{

class CollisionShapeHandle : public handles::Handle
{
public:
	using handles::Handle::Handle;
	
	static const CollisionShapeHandle INVALID;
};

}
}

#endif /* COLLISION_SHAPE_HANDLE_H_ */
