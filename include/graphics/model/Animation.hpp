#ifndef GANIMATION_H_
#define GANIMATION_H_

#include <map>
#include <string>

#include "Types.hpp"

#include "graphics/model/AnimatedBoneNode.hpp"

namespace ice_engine
{
namespace graphics
{
namespace model
{

struct Animation
{
	Animation() = default;

	Animation(
		std::string name,
		float64 duration,
		float64 ticksPerSecond,
		std::map< std::string, AnimatedBoneNode > animatedBoneNodes
	)
	:
		name(name),
		duration(duration),
		ticksPerSecond(ticksPerSecond),
		animatedBoneNodes(animatedBoneNodes)
	{

	}

	std::string name;
	float64 duration;
	float64 ticksPerSecond;
	std::map< std::string, AnimatedBoneNode > animatedBoneNodes;
};

}
}
}

#endif /* ANIMATION_H_ */
