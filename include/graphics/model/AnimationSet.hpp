#ifndef ANIMATIONSET_H_
#define ANIMATIONSET_H_

#include <unordered_map>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "graphics/model/BoneNode.hpp"
#include "graphics/model/Animation.hpp"

namespace ice_engine
{
namespace graphics
{
namespace model
{

struct AnimationSet
{
	std::string name;
	
	BoneNode rootBoneNode;
	std::unordered_map< std::string, Animation > animations;
	glm::mat4 globalInverseTransformation = glm::mat4(1.0f);
};

}
}
}

#endif /* ANIMATIONSET_H_ */
