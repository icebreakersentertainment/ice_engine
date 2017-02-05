#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <map>
#include <string>

#include "Types.hpp"

#include "model/AnimatedBoneNode.hpp"

namespace hercules
{
namespace model
{

struct Animation
{
	std::string name;
	float64 duration;
	float64 ticksPerSecond;
	std::map< std::string, AnimatedBoneNode > animatedBoneNodes;
};

}
}

#endif /* ANIMATION_H_ */
