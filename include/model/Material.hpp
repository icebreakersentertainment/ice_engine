#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace model
{

struct Material
{
	std::string name;
	//GLenum fill_mode;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 emission;
	glm::detail::float32 shininess;
	glm::detail::float32 strength;
};

}

#endif /* MATERIAL_H_ */
