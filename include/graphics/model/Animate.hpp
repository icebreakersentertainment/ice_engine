#ifndef gANIMATE_H_
#define gANIMATE_H_

#include <vector>
#include <unordered_map>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

#include "graphics/model/BoneNode.hpp"
#include "graphics/model/BoneData.hpp"
#include "graphics/model/AnimatedBoneNode.hpp"

namespace ice_engine
{
namespace graphics
{
namespace model
{

void animateSkeleton(
	std::vector< glm::mat4 >& transformations,
	const glm::mat4& globalInverseTransformation,
	const std::unordered_map< std::string, AnimatedBoneNode >& animatedBoneNodes,
	const BoneNode& rootBoneNode,
	const BoneData& boneData,
    const float64 duration,
    const float64 ticksPerSecond,
    const float32 runningTime,
    const uint32 startFrame = 0,
    const uint32 endFrame = 0
);

void animateSkeleton(
	std::vector< glm::mat4 >& transformations,
	const glm::mat4& globalInverseTransformation,
	const std::unordered_map< std::string, AnimatedBoneNode >& animatedBoneNodes,
	const BoneNode& rootBoneNode,
	const BoneData& boneData,
    const float64 duration,
    const float64 ticksPerSecond,
    const float32 runningTime,
	std::vector<uint32>& indexCache,
    const uint32 startFrame = 0,
    const uint32 endFrame = 0
);

}
}
}

#endif /* ANIMATE_H_ */
