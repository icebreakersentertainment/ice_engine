#ifndef ANIMATIONSET_H_
#define ANIMATIONSET_H_

#include <map>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "graphics/model/BoneNode.hpp"
#include "graphics/model/Animation.hpp"

namespace hercules
{
namespace graphics
{
namespace model
{

struct AnimationSet
{
	std::string name;
	
	BoneNode rootBoneNode;
	std::map< std::string, Animation > animations;
	glm::mat4 globalInverseTransformation;
};

}
}
}

#endif /* ANIMATIONSET_H_ */
