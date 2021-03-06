#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "detail/Assert.hpp"

#include "exceptions/RuntimeException.hpp"

#include "Animate.hpp"

namespace ice_engine
{
namespace
{

uint32 findPosition(const float32 animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
{
	ICE_ENGINE_ASSERT(animatedBoneNode.positionTimes.size() > 0);
	
	//if (indexCache[0] >= animatedBoneNode.positionTimes.size())
	//{
	//	indexCache[0] = 0;
	//}
	
	// Not all animated bone nodes have the same number of 'times', so sometimes the cache index value can
	// be larger than the size, but that's ok, it will reset to a valid value once the second for loop runs.
	//ICE_ENGINE_ASSERT( indexCache[0] >= 0 && indexCache[0] < animatedBoneNode.positionTimes.size() );
	
	// Search first from cache position
//	for (uint32 i = indexCache[0]; i < animatedBoneNode.positionTimes.size() - 1; ++i)
//	{
//		if (animationTime < (float32)animatedBoneNode.positionTimes[i + 1])
//		{
//			indexCache[0] = i;
//			return i;
//		}
//	}
	
	// If not found, search from the beginning
	for (uint32 i = 0; i < animatedBoneNode.positionTimes.size() - 1; ++i)
	{
		if (animationTime < (float32)animatedBoneNode.positionTimes[i + 1])
		{
			indexCache[0] = i;
			return i;
		}
	}

	throw RuntimeException("Unable to find appropriate position time - this shouldn't happen.");
}

uint32 findRotation(const float32 animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
{
	ICE_ENGINE_ASSERT(animatedBoneNode.rotationTimes.size() > 0);
	
	//if (indexCache[1] >= animatedBoneNode.rotationTimes.size())
	//{
	//	indexCache[1] = 0;
	//}
	
	// Not all animated bone nodes have the same number of 'times', so sometimes the cache index value can
	// be larger than the size, but that's ok, it will reset to a valid value once the second for loop runs.
	//ICE_ENGINE_ASSERT( indexCache[1] >= 0 && indexCache[1] < animatedBoneNode.rotationTimes.size() );
	
	// Search first from cache position
//	for (uint32 i = indexCache[1]; i < animatedBoneNode.rotationTimes.size() - 1; ++i)
//	{
//		if (animationTime < (float32)animatedBoneNode.rotationTimes[i + 1])
//		{
//			indexCache[1] = i;
//			return i;
//		}
//	}
	
	// If not found, search from the beginning
	for (uint32 i = 0; i < animatedBoneNode.rotationTimes.size() - 1; ++i)
	{
		if (animationTime < (float32)animatedBoneNode.rotationTimes[i + 1])
		{
			indexCache[1] = i;
			return i;
		}
	}

	throw RuntimeException("Unable to find appropriate rotation time - this shouldn't happen.");
}


uint32 findScaling(const float32 animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
{
	ICE_ENGINE_ASSERT(animatedBoneNode.scalingTimes.size() > 0);
	
	//if (indexCache[2] >= animatedBoneNode.scalingTimes.size())
	//{
	//	indexCache[2] = 0;
	//}
	
	// Not all animated bone nodes have the same number of 'times', so sometimes the cache index value can
	// be larger than the size, but that's ok, it will reset to a valid value once the second for loop runs.
	//ICE_ENGINE_ASSERT( indexCache[2] >= 0 && indexCache[2] < animatedBoneNode.scalingTimes.size() );
	
	// Search first from cache position
//	for (uint32 i = indexCache[2]; i < animatedBoneNode.scalingTimes.size() - 1; ++i)
//	{
//		if (animationTime < (float32)animatedBoneNode.scalingTimes[i + 1])
//		{
//			indexCache[2] = i;
//			return i;
//		}
//	}
	
	// If not found, search from the beginning
	for (uint32 i = 0; i < animatedBoneNode.scalingTimes.size() - 1; ++i)
	{
		if (animationTime < (float32)animatedBoneNode.scalingTimes[i + 1])
		{
			indexCache[2] = i;
			return i;
		}
	}

	throw RuntimeException("Unable to find appropriate scaling time - this shouldn't happen.");
}

glm::vec3 calcInterpolatedPosition(const float32 animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
{
	glm::vec3 result;

	if (animatedBoneNode.positionTimes.size() == 1)
	{
		result = animatedBoneNode.positions[0];
		return result;
	}
			
	const uint32 positionIndex = findPosition(animationTime, animatedBoneNode, indexCache);
	const uint32 nextPositionIndex = (positionIndex + 1);
	
	ICE_ENGINE_ASSERT(nextPositionIndex < animatedBoneNode.positionTimes.size());
	
	const float32 deltaTime = (float32)(animatedBoneNode.positionTimes[nextPositionIndex] - animatedBoneNode.positionTimes[positionIndex]);
	const float32 factor = (animationTime - (float32)animatedBoneNode.positionTimes[positionIndex]) / deltaTime;
	
	ICE_ENGINE_ASSERT(factor >= 0.0f && factor <= 1.0f);
	const glm::vec3& start = animatedBoneNode.positions[positionIndex];
	const glm::vec3& end = animatedBoneNode.positions[nextPositionIndex];
	const glm::vec3 delta = end - start;
	result = start + factor * delta;

	return result;
}


glm::quat calcInterpolatedRotation(const float32 animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
{
	glm::quat result;

	// we need at least two values to interpolate...
	if (animatedBoneNode.rotationTimes.size() == 1)
	{
		result = animatedBoneNode.rotations[0];
		return result;
	}

	const uint32 rotationIndex = findRotation(animationTime, animatedBoneNode, indexCache);
	const uint32 nextRotationIndex = (rotationIndex + 1);
	
	ICE_ENGINE_ASSERT(nextRotationIndex < animatedBoneNode.rotationTimes.size());
	
	const float32 deltaTime = (float32)(animatedBoneNode.rotationTimes[nextRotationIndex] - animatedBoneNode.rotationTimes[rotationIndex]);
	const float32 factor = (animationTime - (float32)animatedBoneNode.rotationTimes[rotationIndex]) / deltaTime;
	ICE_ENGINE_ASSERT(factor >= 0.0f && factor <= 1.0f);
	const glm::quat& startRotationQuat = animatedBoneNode.rotations[rotationIndex];
	const glm::quat& endRotationQuat   = animatedBoneNode.rotations[nextRotationIndex];
	
	result = glm::slerp(startRotationQuat, endRotationQuat, factor);
	// TODO: I might not need to normalize the quaternion here...might already have been done in glm::mix??
	result = glm::normalize(result);

	return result;
}


glm::vec3 calcInterpolatedScaling(const float32 animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
{
	glm::vec3 result;

	if (animatedBoneNode.scalingTimes.size() == 1)
	{
		result = animatedBoneNode.scalings[0];
		return result;
	}

	const uint32 scalingIndex = findScaling(animationTime, animatedBoneNode, indexCache);
	const uint32 nextScalingIndex = (scalingIndex + 1);
	
	ICE_ENGINE_ASSERT(nextScalingIndex < animatedBoneNode.scalingTimes.size());
	
	const float32 deltaTime = (float32)(animatedBoneNode.scalingTimes[nextScalingIndex] - animatedBoneNode.scalingTimes[scalingIndex]);
	const float32 factor = (animationTime - (float32)animatedBoneNode.scalingTimes[scalingIndex]) / deltaTime;
	ICE_ENGINE_ASSERT(factor >= 0.0f && factor <= 1.0f);
	const glm::vec3& start = animatedBoneNode.scalings[scalingIndex];
	const glm::vec3& end   = animatedBoneNode.scalings[nextScalingIndex];
    const glm::vec3 delta = end - start;
	result = start + factor * delta;

	return result;
}

glm::mat4 calculateNodeTransformation(const float32 animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
{
	// Interpolate scaling and generate scaling transformation matrix
	const glm::vec3 scaling = calcInterpolatedScaling(animationTime, animatedBoneNode, indexCache);
	const glm::mat4 scalingM = glm::scale( glm::mat4(1.0f), glm::vec3(scaling.x, scaling.y, scaling.z) );

	// Interpolate rotation and generate rotation transformation matrix
	const glm::quat rotationQ = calcInterpolatedRotation(animationTime, animatedBoneNode, indexCache);
	const glm::mat4 rotationM = glm::mat4_cast(rotationQ);

	// Interpolate translation and generate translation transformation matrix
	const glm::vec3 translation = calcInterpolatedPosition(animationTime, animatedBoneNode, indexCache);
	const glm::mat4 translationM = glm::translate( glm::mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z) );

	// Combine the above transformations
	return translationM * rotationM * scalingM;
}

void readNodeHeirarchy(std::vector<glm::mat4>& transformations, const float32 animationTime, const glm::mat4& globalInverseTransform, const std::unordered_map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, const glm::mat4& parentTransform, std::vector<uint32>& indexCache)
{
	glm::mat4 nodeTransformation = glm::mat4( rootBoneNode.transformation );

	{
        const auto it = animatedBoneNodes.find(rootBoneNode.name);

		if (it != animatedBoneNodes.end())
		{
			const AnimatedBoneNode& animatedBoneNode = it->second;
//			const float32 animationTimeTemp = animationTime;

			nodeTransformation = calculateNodeTransformation(animationTime, animatedBoneNode, indexCache);
		}
	}

	const glm::mat4 globalTransformation = parentTransform * nodeTransformation;

	{
        const auto it = boneData.boneIndexMap.find(rootBoneNode.name);

		if (it != boneData.boneIndexMap.end())
		{
            const uint32 boneIndex = it->second;
			transformations[boneIndex] = globalInverseTransform * globalTransformation * boneData.boneTransform[boneIndex].boneOffset;
		}
	}

    for (const auto& boneNode : rootBoneNode.children)
	{
		readNodeHeirarchy(transformations, animationTime, globalInverseTransform, animatedBoneNodes, boneNode, boneData, globalTransformation, indexCache);
	}
}

void readNodeHeirarchy(std::vector<glm::mat4>& transformations, const float32 animationTime, const glm::mat4& globalInverseTransform, const std::unordered_map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, const uint32 startFrame, const uint32 endFrame, const glm::mat4& parentTransform, std::vector<uint32>& indexCache)
{
	glm::mat4 nodeTransformation = glm::mat4(rootBoneNode.transformation);

	// animatedBoneNodes = Animation
	// rootBoneNode = Joint

	{
        const auto it = animatedBoneNodes.find(rootBoneNode.name);

		if (it != animatedBoneNodes.end())
		{
			const AnimatedBoneNode& animatedBoneNode = it->second;

			// Clamp animation time between start and end frame
			if (startFrame > 0 || endFrame > 0)
			{
				if (startFrame < animatedBoneNode.positionTimes.size() && endFrame < animatedBoneNode.positionTimes.size())
				{
					const float32 st = (float32)animatedBoneNode.positionTimes[startFrame];
                    const float32 et = (float32)animatedBoneNode.positionTimes[endFrame];

//					std::cout << "A " << clampedAnimationTime << " " << startFrame << " " << st << " " << endFrame << " " << et << std::endl;

                    const float32 clampedAnimationTime = fmod(animationTime, et - st) + st;

//					std::cout << "B " << animationTimeTemp2 << " " << clampedAnimationTime << " " << startFrame << " " << st << " " << endFrame << " " << et << std::endl;

//					if (st == et)
//					{
//						std::cout << rootBoneNode.name << std::endl;
//						for (uint32 i = 0; i < animatedBoneNode.positionTimes.size(); ++i)
//						{
//							std::cout << i << " : " << animatedBoneNode.positionTimes[i] << std::endl;
//						}
//					}

//					std::cout << "clampedAnimationTime " << clampedAnimationTime << std::endl;

					nodeTransformation = calculateNodeTransformation(clampedAnimationTime, animatedBoneNode, indexCache);
				}
			}
			else
			{
//				std::cout << "animationTimeTemp " << animationTimeTemp << std::endl;

				nodeTransformation = calculateNodeTransformation(animationTime, animatedBoneNode, indexCache);
			}
		}
	}

    const glm::mat4 globalTransformation = parentTransform * nodeTransformation;

	{
        const auto it = boneData.boneIndexMap.find(rootBoneNode.name);

		if (it != boneData.boneIndexMap.end())
		{
            const uint32 boneIndex = it->second;
			transformations[boneIndex] = globalInverseTransform * globalTransformation * boneData.boneTransform[boneIndex].boneOffset;
		}
	}

	for (const auto& boneNode : rootBoneNode.children)
	{
		readNodeHeirarchy(transformations, animationTime, globalInverseTransform, animatedBoneNodes, boneNode, boneData, startFrame, endFrame, globalTransformation, indexCache);
	}
}

}

void animateSkeleton(std::vector<glm::mat4>& transformations, const glm::mat4& globalInverseTransformation, const std::unordered_map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, const float64 duration, const float64 ticksPerSecond, const float32 runningTime)
{
	std::vector<uint32> indexCache = std::vector<uint32>( 3, 0 );
	animateSkeleton( transformations, globalInverseTransformation, animatedBoneNodes, rootBoneNode, boneData, duration, ticksPerSecond, runningTime, indexCache );
}

void animateSkeleton(std::vector<glm::mat4>& transformations, const glm::mat4& globalInverseTransformation, const std::unordered_map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, const float64 duration, const float64 ticksPerSecond, const float32 runningTime, const uint32 startFrame, const uint32 endFrame)
{
	std::vector<uint32> indexCache = std::vector<uint32>( 3, 0 );
	animateSkeleton( transformations, globalInverseTransformation, animatedBoneNodes, rootBoneNode, boneData, duration, ticksPerSecond, runningTime, indexCache, startFrame, endFrame );
}

void animateSkeleton(std::vector<glm::mat4>& transformations, const glm::mat4& globalInverseTransformation, const std::unordered_map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, const float64 duration, const float64 ticksPerSecond, const float32 runningTime, std::vector<uint32>& indexCache)
{
	ICE_ENGINE_ASSERT(transformations.size() >= boneData.boneTransform.size());
	ICE_ENGINE_ASSERT(indexCache.size() == 3);

    const float32 timeInTicks = runningTime * ticksPerSecond;
	const float32 animationTime = fmod(timeInTicks, (float32)duration);

//	std::cout << animationTime << std::endl;

	readNodeHeirarchy( transformations, animationTime, globalInverseTransformation, animatedBoneNodes, rootBoneNode, boneData, glm::mat4(1.0f), indexCache );
}

void animateSkeleton(std::vector<glm::mat4>& transformations, const glm::mat4& globalInverseTransformation, const std::unordered_map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, const float64 duration, const float64 ticksPerSecond, const float32 runningTime, std::vector<uint32>& indexCache, const uint32 startFrame, const uint32 endFrame)
{
	ICE_ENGINE_ASSERT(transformations.size() >= boneData.boneTransform.size());
	ICE_ENGINE_ASSERT(indexCache.size() == 3);

    const float32 timeInTicks = runningTime * ticksPerSecond;
	const float32 animationTime = fmod(timeInTicks, (float32)duration);

//	std::cout << animationTime << std::endl;

	readNodeHeirarchy( transformations, animationTime, globalInverseTransformation, animatedBoneNodes, rootBoneNode, boneData, startFrame, endFrame, glm::mat4(1.0f), indexCache );
}

}
