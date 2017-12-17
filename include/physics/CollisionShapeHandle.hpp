#ifndef COLLISION_SHAPE_HANDLE_H_
#define COLLISION_SHAPE_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace physics
{

class CollisionShapeHandle : public handles::Handle<CollisionShapeHandle>
{
public:
	using handles::Handle<CollisionShapeHandle>::Handle;
};

}
}

#endif /* COLLISION_SHAPE_HANDLE_H_ */
