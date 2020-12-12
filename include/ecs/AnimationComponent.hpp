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

	AnimationComponent(AnimationHandle animationHandle, uint32 startFrame, uint32 endFrame)
	:
		animationHandle(animationHandle),
		startFrame(startFrame),
		endFrame(endFrame)
	{
	};
	
	static uint8 id() { return 7; }

	AnimationHandle animationHandle;
	graphics::BonesHandle bonesHandle;
	float32 runningTime = 0.0f;
	float32 speed = 1.0f;
	uint32 startFrame = 0;
	uint32 endFrame = 0;
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
	ar & c.animationHandle & c.bonesHandle & c.runningTime & c.speed & c.startFrame & c.endFrame & c.transformations;
}

}
}

#endif /* ANIMATIONCOMPONENT_H_ */
