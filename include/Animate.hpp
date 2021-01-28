#ifndef ANIMATE_H_
#define ANIMATE_H_

#include <vector>
#include <unordered_map>
#include <string>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

#include "Model.hpp"

namespace ice_engine
{

void animateSkeleton(std::vector< glm::mat4 >& transformations, const glm::mat4& globalInverseTransformation, const std::unordered_map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, std::chrono::duration<float32> duration, float32 ticksPerSecond, std::chrono::duration<float32> runningTime);
void animateSkeleton(std::vector< glm::mat4 >& transformations, const glm::mat4& globalInverseTransformation, const std::unordered_map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, std::chrono::duration<float32> duration, float32 ticksPerSecond, std::chrono::duration<float32> runningTime, uint32 startFrame, uint32 endFrame);

void animateSkeleton(std::vector< glm::mat4 >& transformations, const glm::mat4& globalInverseTransformation, const std::unordered_map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, std::chrono::duration<float32> duration, float32 ticksPerSecond, std::chrono::duration<float32> runningTime, std::vector<uint32>& indexCache);
void animateSkeleton(std::vector< glm::mat4 >& transformations, const glm::mat4& globalInverseTransformation, const std::unordered_map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, std::chrono::duration<float32> duration, float32 ticksPerSecond, std::chrono::duration<float32> runningTime, std::vector<uint32>& indexCache, uint32 startFrame, uint32 endFrame);

}

#endif /* ANIMATE_H_ */
