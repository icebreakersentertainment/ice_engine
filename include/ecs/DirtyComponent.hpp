#ifndef DIRTYCOMPONENT_H_
#define DIRTYCOMPONENT_H_

#include "serialization/SplitMember.hpp"

namespace ice_engine
{
namespace ecs
{

enum DirtyFlags : uint16
{
	DIRTY_SOURCE_SCRIPT				= 1 << 0,
	DIRTY_SOURCE_PHYSICS			= 1 << 1,
	DIRTY_SOURCE_PATHFINDING		= 1 << 2,

	DIRTY_ALL						= 1 << 3,
	DIRTY_POSITION					= 1 << 4,
	DIRTY_ORIENTATION				= 1 << 5,
	DIRTY_RIGID_BODY_OBJECT			= 1 << 6,
	DIRTY_GHOST_OBJECT				= 1 << 7,
	DIRTY_PATHFINDING_AGENT			= 1 << 8,
	DIRTY_AGENT_STATE				= 1 << 9,
	DIRTY_MOVEMENT_REQUEST_STATE	= 1 << 10,
};

struct DirtyComponent
{
	DirtyComponent() = default;
	
	DirtyComponent(uint16 dirty) : dirty(dirty)
	{
	};
	
	uint16 dirty = 0;
};

}
}

namespace boost
{
namespace serialization
{

template<class Archive>
void serialize(Archive& ar, ice_engine::ecs::DirtyComponent& c, const unsigned int version)
{
	boost::serialization::split_free(ar, c, version);
}

template<class Archive>
void save(Archive& ar, const ice_engine::ecs::DirtyComponent& c, const unsigned int version)
{
	//ar & c.ghostObjectHandle.id();
}

template<class Archive>
void load(Archive& ar, ice_engine::ecs::DirtyComponent& c, const unsigned int version)
{

}

}
}

#endif /* DIRTYCOMPONENT_H_ */
