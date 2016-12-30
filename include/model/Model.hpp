#ifndef MODEL_H_
#define MODEL_H_

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "model/Mesh.hpp"
#include "model/Material.hpp"
#include "model/Texture.hpp"
#include "model/BoneData.hpp"
#include "model/Animation.hpp"
#include "model/BoneNode.hpp"

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

#endif /* MODEL_H_ */
