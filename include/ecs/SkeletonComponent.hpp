#ifndef SKELETONCOMPONENT_H_
#define SKELETONCOMPONENT_H_

#include "SkeletonHandle.hpp"

#include "ecs/Serialization.hpp"

namespace ice_engine
{
namespace ecs
{

struct SkeletonComponent
{
	SkeletonComponent() = default;

	SkeletonComponent(SkeletonHandle skeletonHandle) : skeletonHandle(skeletonHandle)
	{
	};
	
	static uint8 id()  { return 8; }

	SkeletonHandle skeletonHandle;
};

}
}

namespace boost
{
namespace serialization
{

template<class Archive>
void serialize(Archive& ar, ice_engine::ecs::SkeletonComponent& c, const unsigned int version)
{
	ar & c.skeletonHandle;
}

}
}

#endif /* SKELETONCOMPONENT_H_ */
