#ifndef BONEDATA_H_
#define BONEDATA_H_

#include <string>
#include <map>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Bone.hpp"

namespace model
{

struct BoneData
{
	std::string name;
	std::map< std::string, glm::detail::uint32 > boneIndexMap;
	std::vector< Bone > boneTransform;
};

}

#endif /* BONEDATA_H_ */
