#ifndef GHOST_OBJECT_HANDLE_H_
#define GHOST_OBJECT_HANDLE_H_

#include "handles/PointerHandle.hpp"

namespace ice_engine
{
namespace physics
{

class GhostObjectHandle : public handles::PointerHandle<GhostObjectHandle>
{
public:
	using handles::PointerHandle<GhostObjectHandle>::PointerHandle;
	
	static const GhostObjectHandle INVALID;
};

}
}

namespace std
{
	template <>
	struct hash<ice_engine::physics::GhostObjectHandle>
	{
		std::size_t operator()(const ice_engine::physics::GhostObjectHandle& k) const noexcept
		{
			return hash<const void*>{}(k.get());
		}
	};
}

#endif /* GHOST_OBJECT_HANDLE_H_ */
