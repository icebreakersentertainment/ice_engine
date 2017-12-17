#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "graphics/model/Animate.hpp"

namespace ice_engine
{
namespace graphics
{
namespace model
{
namespace
{

uint32 findPosition(float32 animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
{
	assert(animatedBoneNode.positionTimes.size() > 0);
	
	//if (indexCache[0] >= animatedBoneNode.positionTimes.size())
	//{
	//	indexCache[0] = 0;
	//}
	
	// Not all animated bone nodes have the same number of 'times', so sometimes the cache index value can
	// be larger than the size, but that's ok, it will reset to a valid value once the second for loop runs.
	//assert( indexCache[0] >= 0 && indexCache[0] < animatedBoneNode.positionTimes.size() );
	
	// Search first from cache position
	for (uint32 i = indexCache[0]; i < animatedBoneNode.positionTimes.size() - 1; i++)
	{
		if (animationTime < (float32)animatedBoneNode.positionTimes[i + 1])
		{
			indexCache[0] = i;
			return i;
		}
	}
	
	// If not found, search from the beginning
	for (uint32 i = 0; i < animatedBoneNode.positionTimes.size() - 1; i++)
	{
		if (animationTime < (float32)animatedBoneNode.positionTimes[i + 1])
		{
			indexCache[0] = i;
			return i;
		}
	}

	std::string msg = std::string("Unable to find appropriate position time - this shouldn't happen.");
	throw std::runtime_error( msg );
}

uint32 findRotation(float32 animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
{
	assert(animatedBoneNode.rotationTimes.size() > 0);
	
	//if (indexCache[1] >= animatedBoneNode.rotationTimes.size())
	//{
	//	indexCache[1] = 0;
	//}
	
	// Not all animated bone nodes have the same number of 'times', so sometimes the cache index value can
	// be larger than the size, but that's ok, it will reset to a valid value once the second for loop runs.
	//assert( indexCache[1] >= 0 && indexCache[1] < animatedBoneNode.rotationTimes.size() );
	
	// Search first from cache position
	for (uint32 i = indexCache[1]; i < animatedBoneNode.rotationTimes.size() - 1; i++)
	{
		if (animationTime < (float32)animatedBoneNode.rotationTimes[i + 1])
		{
			indexCache[1] = i;
			return i;
		}
	}
	
	// If not found, search from the beginning
	for (uint32 i = 0; i < animatedBoneNode.rotationTimes.size() - 1; i++)
	{
		if (animationTime < (float32)animatedBoneNode.rotationTimes[i + 1])
		{
			indexCache[1] = i;
			return i;
		}
	}
	
	std::string msg = std::string("Unable to find appropriate rotation time - this shouldn't happen.");
	throw std::runtime_error( msg );
}


uint32 findScaling(float32 animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
{
	assert(animatedBoneNode.scalingTimes.size() > 0);
	
	//if (indexCache[2] >= animatedBoneNode.scalingTimes.size())
	//{
	//	indexCache[2] = 0;
	//}
	
	// Not all animated bone nodes have the same number of 'times', so sometimes the cache index value can
	// be larger than the size, but that's ok, it will reset to a valid value once the second for loop runs.
	//assert( indexCache[2] >= 0 && indexCache[2] < animatedBoneNode.scalingTimes.size() );
	
	// Search first from cache position
	for (uint32 i = indexCache[2]; i < animatedBoneNode.scalingTimes.size() - 1; i++)
	{
		if (animationTime < (float32)animatedBoneNode.scalingTimes[i + 1])
		{
			indexCache[2] = i;
			return i;
		}
	}
	
	// If not found, search from the beginning
	for (uint32 i = 0; i < animatedBoneNode.scalingTimes.size() - 1; i++)
	{
		if (animationTime < (float32)animatedBoneNode.scalingTimes[i + 1])
		{
			indexCache[2] = i;
			return i;
		}
	}
	
	std::string msg = std::string("Unable to find appropriate scaling time - this shouldn't happen.");
	throw std::runtime_error( msg );
}

void calcInterpolatedPosition(glm::vec3& out, float32 animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
{
	if (animatedBoneNode.positionTimes.size() == 1)
	{
		out = animatedBoneNode.positions[0];
		return;
	}
			
	uint32 positionIndex = findPosition(animationTime, animatedBoneNode, indexCache);
	uint32 nextPositionIndex = (positionIndex + 1);
	
	assert(nextPositionIndex < animatedBoneNode.positionTimes.size());
	
	float32 deltaTime = (float32)(animatedBoneNode.positionTimes[nextPositionIndex] - animatedBoneNode.positionTimes[positionIndex]);
	float32 factor = (animationTime - (float32)animatedBoneNode.positionTimes[positionIndex]) / deltaTime;
	
	assert(factor >= 0.0f && factor <= 1.0f);
	const glm::vec3& start = animatedBoneNode.positions[positionIndex];
	const glm::vec3& end = animatedBoneNode.positions[nextPositionIndex];
	glm::vec3 delta = end - start;
	out = start + factor * delta;
}


void calcInterpolatedRotation(glm::quat& out, float32 animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
{
	// we need at least two values to interpolate...
	if (animatedBoneNode.rotationTimes.size() == 1)
	{
		out = animatedBoneNode.rotations[0];
		return;
	}

	uint32 rotationIndex = findRotation(animationTime, animatedBoneNode, indexCache);
	uint32 nextRotationIndex = (rotationIndex + 1);
	
	assert(nextRotationIndex < animatedBoneNode.rotationTimes.size());
	
	float32 deltaTime = (float32)(animatedBoneNode.rotationTimes[nextRotationIndex] - animatedBoneNode.rotationTimes[rotationIndex]);
	float32 factor = (animationTime - (float32)animatedBoneNode.rotationTimes[rotationIndex]) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	const glm::quat& startRotationQuat = animatedBoneNode.rotations[rotationIndex];
	const glm::quat& endRotationQuat   = animatedBoneNode.rotations[nextRotationIndex];
	
	out = glm::slerp(startRotationQuat, endRotationQuat, factor);
	// TODO: I might not need to normalize the quaternion here...might already have been done in glm::mix??
	out = glm::normalize( out );
}


void calcInterpolatedScaling(glm::vec3& out, float32 animationTime, const AnimatedBoneNode& animatedBoneNode, std::vector<uint32>& indexCache)
{
	if (animatedBoneNode.scalingTimes.size() == 1)
	{
		out = animatedBoneNode.scalings[0];
		return;
	}

	uint32 scalingIndex = findScaling(animationTime, animatedBoneNode, indexCache);
	uint32 nextScalingIndex = (scalingIndex + 1);
	
	assert(nextScalingIndex < animatedBoneNode.scalingTimes.size());
	
	float32 deltaTime = (float32)(animatedBoneNode.scalingTimes[nextScalingIndex] - animatedBoneNode.scalingTimes[scalingIndex]);
	float32 factor = (animationTime - (float32)animatedBoneNode.scalingTimes[scalingIndex]) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	const glm::vec3& start = animatedBoneNode.scalings[scalingIndex];
	const glm::vec3& end   = animatedBoneNode.scalings[nextScalingIndex];
	glm::vec3 delta = end - start;
	out = start + factor * delta;
}

void readNodeHeirarchy(std::vector< glm::mat4 >& transformations, float32 animationTime, const glm::mat4& globalInverseTransform, const std::map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, uint32 startFrame, uint32 endFrame, const glm::mat4& parentTransform, std::vector<uint32>& indexCache)
{
	glm::mat4 nodeTransformation = glm::mat4( rootBoneNode.transformation );
	
	{
		auto it = animatedBoneNodes.find( rootBoneNode.name );

		if ( it != animatedBoneNodes.end() )
		{
			const AnimatedBoneNode& animatedBoneNode = it->second;

			// Clamp animation time between start and end frame
			if ( startFrame > 0 || endFrame > 0)
			{
				float32 st = (float32)animatedBoneNode.positionTimes[startFrame];
				float32 et = (float32)animatedBoneNode.positionTimes[endFrame];

				animationTime = fmod(animationTime, et) + st;
			}
			
			// Interpolate scaling and generate scaling transformation matrix
			glm::vec3 scaling;
			calcInterpolatedScaling(scaling, animationTime, animatedBoneNode, indexCache);
			glm::mat4 scalingM = glm::scale( glm::mat4(1.0f), glm::vec3(scaling.x, scaling.y, scaling.z) );
			
			// Interpolate rotation and generate rotation transformation matrix
			glm::quat rotationQ;
			calcInterpolatedRotation(rotationQ, animationTime, animatedBoneNode, indexCache);
			glm::mat4 rotationM = glm::mat4_cast(rotationQ);
	
			// Interpolate translation and generate translation transformation matrix
			glm::vec3 translation;
			calcInterpolatedPosition(translation, animationTime, animatedBoneNode, indexCache);
			glm::mat4 translationM = glm::translate( glm::mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z) );
			
			// Combine the above transformations
			nodeTransformation = translationM * rotationM * scalingM;
		}
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransformation;
	
	{
		auto it = boneData.boneIndexMap.find( rootBoneNode.name );
		
		if ( it != boneData.boneIndexMap.end() )
		{
			uint32 boneIndex = it->second;
			transformations[boneIndex] = globalInverseTransform * globalTransformation * boneData.boneTransform[boneIndex].boneOffset;
		}
	}
	
	for (uint32 i = 0; i < rootBoneNode.children.size(); i++)
	{
		readNodeHeirarchy(transformations, animationTime, globalInverseTransform, animatedBoneNodes, rootBoneNode.children[i], boneData, startFrame, endFrame, globalTransformation, indexCache);
	}
}

}

void animateSkeleton(std::vector< glm::mat4 >& transformations, const glm::mat4& globalInverseTransformation, const std::map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, float64 duration, float64 ticksPerSecond, float32 runningTime, uint32 startFrame, uint32 endFrame)
{
	std::vector<uint32> indexCache = std::vector<uint32>( 3 );
	animateSkeleton( transformations, globalInverseTransformation, animatedBoneNodes, rootBoneNode, boneData, duration, ticksPerSecond, runningTime, indexCache, startFrame, endFrame );
}

void animateSkeleton(std::vector< glm::mat4 >& transformations, const glm::mat4& globalInverseTransformation, const std::map< std::string, AnimatedBoneNode >& animatedBoneNodes, const BoneNode& rootBoneNode, const BoneData& boneData, float64 duration, float64 ticksPerSecond, float32 runningTime, std::vector<uint32>& indexCache, uint32 startFrame, uint32 endFrame)
{
	assert(transformations.size() >= boneData.boneTransform.size());
	assert( indexCache.size() == 3 );

	float32 timeInTicks = runningTime * ticksPerSecond;
	float32 animationTime = fmod(timeInTicks, (float32)duration);
	
	readNodeHeirarchy( transformations, animationTime, globalInverseTransformation, animatedBoneNodes, rootBoneNode, boneData, startFrame, endFrame, glm::mat4(), indexCache );
}

}
}
}
