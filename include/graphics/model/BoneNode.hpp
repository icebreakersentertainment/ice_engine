#ifndef BONENODE_H_
#define BONENODE_H_

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
	glm::mat4 transformation;
	std::vector< BoneNode > children;
};

}
}
}

#endif /* BONENODE_H_ */
