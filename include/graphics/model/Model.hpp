#ifndef MODEL_H_
#define MODEL_H_

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "graphics/model/Mesh.hpp"
#include "graphics/model/Material.hpp"
#include "graphics/model/Texture.hpp"
#include "graphics/model/BoneData.hpp"
#include "graphics/model/Animation.hpp"
#include "graphics/model/BoneNode.hpp"

namespace hercules
{
namespace graphics
{
namespace model
{

struct Model
{
	std::vector<Mesh> meshes;
	std::vector<Material> materials;
	std::vector<Texture> textures;
	std::vector<BoneData> boneData;
	std::vector<Animation> animations;
	BoneNode rootBoneNode;
	glm::mat4 globalInverseTransformation;
};

}
}
}

#endif /* MODEL_H_ */
