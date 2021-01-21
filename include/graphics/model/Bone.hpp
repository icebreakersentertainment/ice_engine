#ifndef GBONE_H_
#define GBONE_H_

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
	glm::mat4 boneOffset = glm::mat4(1.0f);
};

}
}
}

#endif /* BONE_H_ */
