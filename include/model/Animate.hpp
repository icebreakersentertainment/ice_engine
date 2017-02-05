#ifndef ANIMATE_H_
#define ANIMATE_H_

#include <vector>
#include <map>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

#include "model/BoneNode.hpp"
#include "model/BoneData.hpp"
#include "model/AnimatedBoneNode.hpp"

namespace hercules
{
namespace model
{

void animateSkeleton(std::vector< glm::mat4 >& transformations, const glm::mat4& globalInverseTransformation, const std::map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, float64 duration, float64 ticksPerSecond, float32 runningTime, uint32 startFrame = 0, uint32 endFrame = 0);

void animateSkeleton(std::vector< glm::mat4 >& transformations, const glm::mat4& globalInverseTransformation, const std::map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, float64 duration, float64 ticksPerSecond, float32 runningTime, std::vector<uint32>& indexCache, uint32 startFrame = 0, uint32 endFrame = 0);

}
}

#endif /* ANIMATE_H_ */
