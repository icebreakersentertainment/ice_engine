#ifndef gBONENODE_H_
#define gBONENODE_H_

#include <string>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "graphics/model/BoneNode.hpp"

namespace ice_engine
{
namespace graphics
{
namespace model
{

struct BoneNode
{
	std::string name;
	glm::mat4 transformation = glm::mat4(1.0f);
	std::vector<BoneNode> children;
};

}
}
}

#endif /* BONENODE_H_ */
