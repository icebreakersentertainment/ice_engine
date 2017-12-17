#ifndef BONE_H_
#define BONE_H_

#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace ice_engine
{
namespace graphics
{
namespace model
{

struct Bone
{
	std::string name;
	glm::mat4 boneOffset;
};

}
}
}

#endif /* BONE_H_ */
