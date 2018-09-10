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

namespace std
{
	template <>
	struct hash<ice_engine::physics::CollisionShapeHandle>
	{
		std::size_t operator()(const ice_engine::physics::CollisionShapeHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* COLLISION_SHAPE_HANDLE_H_ */
