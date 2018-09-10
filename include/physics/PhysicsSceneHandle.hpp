#ifndef PHYSICS_SCENE_HANDLE_H_
#define PHYSICS_SCENE_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace physics
{

class PhysicsSceneHandle : public handles::Handle<PhysicsSceneHandle>
{
public:
	using handles::Handle<PhysicsSceneHandle>::Handle;
};

}
}

namespace std
{
	template <>
	struct hash<ice_engine::physics::PhysicsSceneHandle>
	{
		std::size_t operator()(const ice_engine::physics::PhysicsSceneHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* PHYSICS_SCENE_HANDLE_H_ */
