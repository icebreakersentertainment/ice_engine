#ifndef ANIMATE_H_
#define ANIMATE_H_

#include <vector>
#include <map>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "model/BoneNode.hpp"
#include "model/BoneData.hpp"
#include "model/AnimatedBoneNode.hpp"

namespace model
{

void animateSkeleton(std::vector< glm::mat4 >& transformations, const glm::mat4& globalInverseTransformation, const std::map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, glm::detail::float64 duration, glm::detail::float64 ticksPerSecond, glm::detail::float32 runningTime, glm::detail::uint32 startFrame = 0, glm::detail::uint32 endFrame = 0);

void animateSkeleton(std::vector< glm::mat4 >& transformations, const glm::mat4& globalInverseTransformation, const std::map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, glm::detail::float64 duration, glm::detail::float64 ticksPerSecond, glm::detail::float32 runningTime, std::vector<glm::detail::uint32>& indexCache, glm::detail::uint32 startFrame = 0, glm::detail::uint32 endFrame = 0);

}

#endif /* ANIMATE_H_ */
