#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

#include "model/VertexBoneData.hpp"

namespace hercules
{
namespace graphics
{
namespace model
{

struct Mesh
{
	std::string name;
	std::vector< glm::vec3 > vertices;
	std::vector< uint32 > indices;
	std::vector< glm::vec4 > colors;
	std::vector< glm::vec3 > normals;
	std::vector< glm::vec2 > textureCoordinates;
	std::vector< VertexBoneData > bones;
};

}
}
}

#endif /* MESH_H_ */
