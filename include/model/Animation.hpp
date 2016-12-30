#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <map>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "model/AnimatedBoneNode.hpp"

namespace model
{

struct Animation
{
	std::string name;
	glm::detail::float64 duration;
	glm::detail::float64 ticksPerSecond;
	std::map< std::string, AnimatedBoneNode > animatedBoneNodes;
};

}

#endif /* ANIMATION_H_ */
