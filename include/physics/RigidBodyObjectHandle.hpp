#ifndef RIGID_BODY_OBJECT_HANDLE_H_
#define RIGID_BODY_OBJECT_HANDLE_H_

#include "handles/PointerHandle.hpp"

namespace ice_engine
{
namespace physics
{

class RigidBodyObjectHandle : public handles::PointerHandle<RigidBodyObjectHandle>
{
public:
	using handles::PointerHandle<RigidBodyObjectHandle>::PointerHandle;
	
	static const RigidBodyObjectHandle INVALID;
};

}
}

namespace std
{
	template <>
	struct hash<ice_engine::physics::RigidBodyObjectHandle>
	{
		std::size_t operator()(const ice_engine::physics::RigidBodyObjectHandle& k) const noexcept
		{
			return hash<const void*>{}(k.get());
		}
	};
}

#endif /* RIGID_BODY_OBJECT_HANDLE_H_ */
