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

uint32 findPosition(const std::chrono::duration<float32> animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
{
	ICE_ENGINE_ASSERT(animatedBoneNode.positionKeyFrames.size() > 0);
	
	//if (indexCache[0] >= animatedBoneNode.positionKeyFrames.size())
	//{
	//	indexCache[0] = 0;
	//}
	
	// Not all animated bone nodes have the same number of 'times', so sometimes the cache index value can
	// be larger than the size, but that's ok, it will reset to a valid value once the second for loop runs.
	//ICE_ENGINE_ASSERT( indexCache[0] >= 0 && indexCache[0] < animatedBoneNode.positionKeyFrames.size() );
	
	// Search first from cache position
//	for (uint32 i = indexCache[0]; i < animatedBoneNode.positionKeyFrames.size() - 1; ++i)
//	{
//		if (animationTime < (float32)animatedBoneNode.positionKeyFrames[i + 1])
//		{
//			indexCache[0] = i;
//			return i;
//		}
//	}
	
	// If not found, search from the beginning
	for (uint32 i = 0; i < animatedBoneNode.positionKeyFrames.size() - 1; ++i)
	{
		if (animationTime < animatedBoneNode.positionKeyFrames[i + 1].time)
		{
			indexCache[0] = i;
			return i;
		}
	}

	throw RuntimeException("Unable to find appropriate position time - this shouldn't happen.");
}

uint32 findRotation(const std::chrono::duration<float32> animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
{
	ICE_ENGINE_ASSERT(animatedBoneNode.rotationKeyFrames.size() > 0);
	
	//if (indexCache[1] >= animatedBoneNode.rotationKeyFrames.size())
	//{
	//	indexCache[1] = 0;
	//}
	
	// Not all animated bone nodes have the same number of 'times', so sometimes the cache index value can
	// be larger than the size, but that's ok, it will reset to a valid value once the second for loop runs.
	//ICE_ENGINE_ASSERT( indexCache[1] >= 0 && indexCache[1] < animatedBoneNode.rotationKeyFrames.size() );
	
	// Search first from cache position
//	for (uint32 i = indexCache[1]; i < animatedBoneNode.rotationKeyFrames.size() - 1; ++i)
//	{
//		if (animationTime < (float32)animatedBoneNode.rotationKeyFrames[i + 1])
//		{
//			indexCache[1] = i;
//			return i;
//		}
//	}
	
	// If not found, search from the beginning
	for (uint32 i = 0; i < animatedBoneNode.rotationKeyFrames.size() - 1; ++i)
	{
		if (animationTime < animatedBoneNode.rotationKeyFrames[i + 1].time)
		{
			indexCache[1] = i;
			return i;
		}
	}

	throw RuntimeException("Unable to find appropriate rotation time - this shouldn't happen.");
}


uint32 findScaling(const std::chrono::duration<float32> animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
{
	ICE_ENGINE_ASSERT(animatedBoneNode.scalingKeyFrames.size() > 0);
	
	//if (indexCache[2] >= animatedBoneNode.scalingKeyFrames.size())
	//{
	//	indexCache[2] = 0;
	//}
	
	// Not all animated bone nodes have the same number of 'times', so sometimes the cache index value can
	// be larger than the size, but that's ok, it will reset to a valid value once the second for loop runs.
	//ICE_ENGINE_ASSERT( indexCache[2] >= 0 && indexCache[2] < animatedBoneNode.scalingKeyFrames.size() );
	
	// Search first from cache position
//	for (uint32 i = indexCache[2]; i < animatedBoneNode.scalingKeyFrames.size() - 1; ++i)
//	{
//		if (animationTime < (float32)animatedBoneNode.scalingKeyFrames[i + 1])
//		{
//			indexCache[2] = i;
//			return i;
//		}
//	}
	
	// If not found, search from the beginning
	for (uint32 i = 0; i < animatedBoneNode.scalingKeyFrames.size() - 1; ++i)
	{
		if (animationTime < animatedBoneNode.scalingKeyFrames[i + 1].time)
		{
			indexCache[2] = i;
			return i;
		}
	}

	throw RuntimeException("Unable to find appropriate scaling time - this shouldn't happen.");
}

glm::vec3 calcInterpolatedPosition(const std::chrono::duration<float32> animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
{
	glm::vec3 result;

	if (animatedBoneNode.positionKeyFrames.size() == 1)
	{
		result = animatedBoneNode.positionKeyFrames[0].transformation;
		return result;
	}
			
	const uint32 positionIndex = findPosition(animationTime, animatedBoneNode, indexCache);
	const uint32 nextPositionIndex = (positionIndex + 1);
	
	ICE_ENGINE_ASSERT(nextPositionIndex < animatedBoneNode.positionKeyFrames.size());
	
	const std::chrono::duration<float32> deltaTime = (animatedBoneNode.positionKeyFrames[nextPositionIndex].time - animatedBoneNode.positionKeyFrames[positionIndex].time);
	const std::chrono::duration<float32> factor = std::chrono::duration<float32>((animationTime - animatedBoneNode.positionKeyFrames[positionIndex].time) / deltaTime);
	
	ICE_ENGINE_ASSERT(factor.count() >= 0.0f && factor.count() <= 1.0f);
	const glm::vec3& start = animatedBoneNode.positionKeyFrames[positionIndex].transformation;
	const glm::vec3& end = animatedBoneNode.positionKeyFrames[nextPositionIndex].transformation;
	const glm::vec3 delta = end - start;
	result = start + factor.count() * delta;

	return result;
}


glm::quat calcInterpolatedRotation(const std::chrono::duration<float32> animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
{
	glm::quat result;

	// we need at least two values to interpolate...
	if (animatedBoneNode.rotationKeyFrames.size() == 1)
	{
		result = animatedBoneNode.rotationKeyFrames[0].transformation;
		return result;
	}

	const uint32 rotationIndex = findRotation(animationTime, animatedBoneNode, indexCache);
	const uint32 nextRotationIndex = (rotationIndex + 1);
	
	ICE_ENGINE_ASSERT(nextRotationIndex < animatedBoneNode.rotationKeyFrames.size());
	
	const std::chrono::duration<float32> deltaTime = (animatedBoneNode.rotationKeyFrames[nextRotationIndex].time - animatedBoneNode.rotationKeyFrames[rotationIndex].time);
	const std::chrono::duration<float32> factor = std::chrono::duration<float32>((animationTime - animatedBoneNode.rotationKeyFrames[rotationIndex].time) / deltaTime);
	ICE_ENGINE_ASSERT(factor.count() >= 0.0f && factor.count() <= 1.0f);
	const glm::quat& startRotationQuat = animatedBoneNode.rotationKeyFrames[rotationIndex].transformation;
	const glm::quat& endRotationQuat   = animatedBoneNode.rotationKeyFrames[nextRotationIndex].transformation;
	
	result = glm::slerp(startRotationQuat, endRotationQuat, factor.count());
	// TODO: I might not need to normalize the quaternion here...might already have been done in glm::mix??
	result = glm::normalize(result);

	return result;
}


glm::vec3 calcInterpolatedScaling(const std::chrono::duration<float32> animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
{
	glm::vec3 result;

	if (animatedBoneNode.scalingKeyFrames.size() == 1)
	{
		result = animatedBoneNode.scalingKeyFrames[0].transformation;
		return result;
	}

	const uint32 scalingIndex = findScaling(animationTime, animatedBoneNode, indexCache);
	const uint32 nextScalingIndex = (scalingIndex + 1);
	
	ICE_ENGINE_ASSERT(nextScalingIndex < animatedBoneNode.scalingKeyFrames.size());
	
	const std::chrono::duration<float32> deltaTime = (animatedBoneNode.scalingKeyFrames[nextScalingIndex].time - animatedBoneNode.scalingKeyFrames[scalingIndex].time);
	const std::chrono::duration<float32> factor = std::chrono::duration<float32>((animationTime - animatedBoneNode.scalingKeyFrames[scalingIndex].time) / deltaTime);
	ICE_ENGINE_ASSERT(factor.count() >= 0.0f && factor.count() <= 1.0f);
	const glm::vec3& start = animatedBoneNode.scalingKeyFrames[scalingIndex].transformation;
	const glm::vec3& end   = animatedBoneNode.scalingKeyFrames[nextScalingIndex].transformation;
    const glm::vec3 delta = end - start;
	result = start + factor.count() * delta;

	return result;
}

glm::mat4 calculateNodeTransformation(const std::chrono::duration<float32> animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
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

void readNodeHeirarchy(std::vector<glm::mat4>& transformations, const std::chrono::duration<float32> animationTime, const glm::mat4& globalInverseModelSpaceTransform, const std::unordered_map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, const glm::mat4& parentJointSpaceTransform, std::vector<uint32>& indexCache)
{
	glm::mat4 jointSpaceTransformation = glm::mat4(rootBoneNode.transformation);

	{
        const auto it = animatedBoneNodes.find(rootBoneNode.name);

		if (it != animatedBoneNodes.end())
		{
			const AnimatedBoneNode& animatedBoneNode = it->second;

            jointSpaceTransformation = calculateNodeTransformation(animationTime, animatedBoneNode, indexCache);
		}
	}

	const glm::mat4 modelSpacePoseTansformation = parentJointSpaceTransform * jointSpaceTransformation;

	{
        const auto it = boneData.boneIndexMap.find(rootBoneNode.name);

		if (it != boneData.boneIndexMap.end())
		{
            const uint32 boneIndex = it->second;
			transformations[boneIndex] = globalInverseModelSpaceTransform * modelSpacePoseTansformation * boneData.boneTransform[boneIndex].inverseModelSpacePoseTransform;
		}
	}

    for (const auto& boneNode : rootBoneNode.children)
	{
		readNodeHeirarchy(transformations, animationTime, globalInverseModelSpaceTransform, animatedBoneNodes, boneNode, boneData, modelSpacePoseTansformation, indexCache);
	}
}

void readNodeHeirarchy(std::vector<glm::mat4>& transformations, const std::chrono::duration<float32> animationTime, const glm::mat4& globalInverseTransform, const std::unordered_map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, const uint32 startFrame, const uint32 endFrame, const glm::mat4& parentTransform, std::vector<uint32>& indexCache)
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
				if (startFrame < animatedBoneNode.positionKeyFrames.size() && endFrame < animatedBoneNode.positionKeyFrames.size())
				{
					const std::chrono::duration<float32> st = animatedBoneNode.positionKeyFrames[startFrame].time;
                    const std::chrono::duration<float32> et = animatedBoneNode.positionKeyFrames[endFrame].time;

//					std::cout << "A " << clampedAnimationTime << " " << startFrame << " " << st << " " << endFrame << " " << et << std::endl;

                    const std::chrono::duration<float32> clampedAnimationTime = std::chrono::duration<float32>(fmod(animationTime.count(), (et - st).count())) + st;

//					std::cout << "B " << animationTimeTemp2 << " " << clampedAnimationTime << " " << startFrame << " " << st << " " << endFrame << " " << et << std::endl;

//					if (st == et)
//					{
//						std::cout << rootBoneNode.name << std::endl;
//						for (uint32 i = 0; i < animatedBoneNode.positionKeyFrames.size(); ++i)
//						{
//							std::cout << i << " : " << animatedBoneNode.positionKeyFrames[i] << std::endl;
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
			transformations[boneIndex] = globalInverseTransform * globalTransformation * boneData.boneTransform[boneIndex].inverseModelSpacePoseTransform;
		}
	}

	for (const auto& boneNode : rootBoneNode.children)
	{
		readNodeHeirarchy(transformations, animationTime, globalInverseTransform, animatedBoneNodes, boneNode, boneData, startFrame, endFrame, globalTransformation, indexCache);
	}
}

}

void animateSkeleton(std::vector<glm::mat4>& transformations, const glm::mat4& globalInverseTransformation, const std::unordered_map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, const std::chrono::duration<float32> duration, const float32 ticksPerSecond, const std::chrono::duration<float32> runningTime)
{
	std::vector<uint32> indexCache = std::vector<uint32>(3, 0);
	animateSkeleton( transformations, globalInverseTransformation, animatedBoneNodes, rootBoneNode, boneData, duration, ticksPerSecond, runningTime, indexCache );
}

void animateSkeleton(std::vector<glm::mat4>& transformations, const glm::mat4& globalInverseTransformation, const std::unordered_map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, const std::chrono::duration<float32> duration, const float32 ticksPerSecond, const std::chrono::duration<float32> runningTime, const uint32 startFrame, const uint32 endFrame)
{
	std::vector<uint32> indexCache = std::vector<uint32>(3, 0);
	animateSkeleton( transformations, globalInverseTransformation, animatedBoneNodes, rootBoneNode, boneData, duration, ticksPerSecond, runningTime, indexCache, startFrame, endFrame );
}

void animateSkeleton(std::vector<glm::mat4>& transformations, const glm::mat4& globalInverseTransformation, const std::unordered_map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, const std::chrono::duration<float32> duration, const float32 ticksPerSecond, const std::chrono::duration<float32> runningTime, std::vector<uint32>& indexCache)
{
	ICE_ENGINE_ASSERT(transformations.size() >= boneData.boneTransform.size());
	ICE_ENGINE_ASSERT(indexCache.size() == 3);

    const std::chrono::duration<float32> timeInTicks = runningTime * ticksPerSecond;
	const std::chrono::duration<float32> animationTime = std::chrono::duration<float32>(fmod(timeInTicks.count(), duration.count()));

//	std::cout << animationTime << std::endl;

	readNodeHeirarchy( transformations, animationTime, globalInverseTransformation, animatedBoneNodes, rootBoneNode, boneData, glm::mat4(1.0f), indexCache );
}

void animateSkeleton(std::vector<glm::mat4>& transformations, const glm::mat4& globalInverseTransformation, const std::unordered_map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, const std::chrono::duration<float32> duration, const float32 ticksPerSecond, const std::chrono::duration<float32> runningTime, std::vector<uint32>& indexCache, const uint32 startFrame, const uint32 endFrame)
{
	ICE_ENGINE_ASSERT(transformations.size() >= boneData.boneTransform.size());
	ICE_ENGINE_ASSERT(indexCache.size() == 3);

    const std::chrono::duration<float32> timeInTicks = runningTime * ticksPerSecond;
	const std::chrono::duration<float32> animationTime = std::chrono::duration<float32>(fmod(timeInTicks.count(), duration.count()));

//	std::cout << animationTime << std::endl;

	readNodeHeirarchy( transformations, animationTime, globalInverseTransformation, animatedBoneNodes, rootBoneNode, boneData, startFrame, endFrame, glm::mat4(1.0f), indexCache );
}

}
