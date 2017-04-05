#ifndef ANIMATEDBONENODE_H_
#define ANIMATEDBONENODE_H_

#include <string>
#include <vector>
#include <utility>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "glm/gtc/quaternion.hpp"

#include "Types.hpp"

namespace hercules
{
namespace graphics
{
namespace model
{

struct AnimatedBoneNode
{
	AnimatedBoneNode()
	{}
	AnimatedBoneNode(
		std::string name,
		std::vector< float64 > positionTimes,
		std::vector< float64 > rotationTimes,
		std::vector< float64 > scalingTimes,
		std::vector< glm::vec3 > positions,
		std::vector< glm::quat > rotations,
		std::vector< glm::vec3 > scalings
	) : name(std::move(name)), positionTimes(std::move(positionTimes)), rotationTimes(std::move(rotationTimes)), scalingTimes(std::move(scalingTimes)), positions(std::move(positions)), rotations(std::move(rotations)), scalings(std::move(scalings))
	{}

	std::string name;
	std::vector< float64 > positionTimes;
	std::vector< float64 > rotationTimes;
	std::vector< float64 > scalingTimes;
	std::vector< glm::vec3 > positions;
	std::vector< glm::quat > rotations;
	std::vector< glm::vec3 > scalings;
};

}
}
}

#endif /* ANIMATEDBONENODE_H_ */
