#ifndef ANIMATIONCOMPONENT_H_
#define ANIMATIONCOMPONENT_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "AnimationHandle.hpp"

#include "graphics/BonesHandle.hpp"

#include "serialization/std/Vector.hpp"
#include "serialization/glm/Mat4.hpp"

#include "ecs/Serialization.hpp"

namespace ice_engine
{
namespace ecs
{

struct AnimationComponent
{
	AnimationComponent() = default;

	AnimationComponent(AnimationHandle animationHandle) : animationHandle(animationHandle)
	{
	};
	
	static uint8 id() { return 7; }

	AnimationHandle animationHandle;
	graphics::BonesHandle bonesHandle;
	float32 runningTime = 0.0f;
	std::vector<glm::mat4> transformations;
};

}
}

namespace boost
{
namespace serialization
{

template<class Archive>
void serialize(Archive& ar, ice_engine::ecs::AnimationComponent& c, const unsigned int version)
{
	ar & c.animationHandle & c.bonesHandle & c.runningTime & c.transformations;
}

}
}

#endif /* ANIMATIONCOMPONENT_H_ */
