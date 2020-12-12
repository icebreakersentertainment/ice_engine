#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <vector>
#include <unordered_map>
#include <string>


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <assimp/anim.h>

#include "Skeleton.hpp"

#include "logger/ILogger.hpp"
#include "fs/IFileSystem.hpp"

#include "Types.hpp"

namespace ice_engine
{

struct AnimatedBoneNode
{
	AnimatedBoneNode() = default;

	AnimatedBoneNode(
		std::string name,
		std::vector< float64 > positionTimes,
		std::vector< float64 > rotationTimes,
		std::vector< float64 > scalingTimes,
		std::vector< glm::vec3 > positions,
		std::vector< glm::quat > rotations,
		std::vector< glm::vec3 > scalings
	) : name(std::move(name)), positionTimes(std::move(positionTimes)), rotationTimes(std::move(rotationTimes)), scalingTimes(std::move(scalingTimes)), positions(std::move(positions)), rotations(std::move(rotations)), scalings(std::move(scalings))
	{
	}

	std::string name;
	std::vector< float64 > positionTimes;
	std::vector< float64 > rotationTimes;
	std::vector< float64 > scalingTimes;
	std::vector< glm::vec3 > positions;
	std::vector< glm::quat > rotations;
	std::vector< glm::vec3 > scalings;
};

class Animation
{
public:

	Animation() = default;
	
	Animation(
		std::string name,
		float64 duration,
		float64 ticksPerSecond,
		std::unordered_map< std::string, AnimatedBoneNode > animatedBoneNodes
	)
	:
		name_(std::move(name)),
		duration_(duration),
		ticksPerSecond_(ticksPerSecond),
		animatedBoneNodes_(std::move(animatedBoneNodes))
	{
	}

	Animation(const std::string& name, const std::string& filename, const aiAnimation* animation, logger::ILogger* logger, fs::IFileSystem* fileSystem)
	{
		import(name, filename, animation, logger, fileSystem);
	}

	~Animation() = default;

	const std::string& name() const
	{
		return name_;
	}

	float64 duration() const
	{
		return duration_;
	}

	float64 ticksPerSecond() const
	{
		return ticksPerSecond_;
	}

	const std::unordered_map<std::string, AnimatedBoneNode>& animatedBoneNodes() const
	{
		return animatedBoneNodes_;
	}

private:
	std::string name_;
	float64 duration_ = 0;
	float64 ticksPerSecond_ = 0;
	std::unordered_map< std::string, AnimatedBoneNode > animatedBoneNodes_;

	void import(const std::string& name, const std::string& filename, const aiAnimation* animation, logger::ILogger* logger, fs::IFileSystem* fileSystem)
	{
		assert(scene != nullptr);

        LOG_DEBUG(logger, "Importing animation for mesh '%s' for model '%s'." , filename, name);

		name_ = std::string( animation->mName.C_Str() );

        LOG_DEBUG(logger, "Animation has name '%s' for mesh '%s' for model '%s'." , name_, filename, name);

		// Error check - animations with no name are not allowed
		if (name_.compare( std::string("") ) == 0)
		{
            LOG_WARN(logger, "Animations with no name are not allowed for mesh '%s' for model '%s'." , filename, name);

			name_ = name + "_animation";
            LOG_WARN(logger, "Setting animation name to '%s' for mesh '%s' for model '%s'." , name_, filename, name);
			// TODO: should we throw an exception?
			//throw std::runtime_error(msg);

			// TODO: Should we skip to the next animation?
			//continue;
		}

		duration_ = animation->mDuration;
		ticksPerSecond_ = animation->mTicksPerSecond;

		// Load AnimatedBoneNodes for this animation
		for (uint32 j = 0; j < animation->mNumChannels; ++j)
		{
			const aiNodeAnim* pNodeAnim = animation->mChannels[j];

			AnimatedBoneNode abn;
			abn.name = std::string( pNodeAnim->mNodeName.C_Str() );

			for (uint32 k = 0; k < pNodeAnim->mNumPositionKeys; ++k)
			{
				abn.positionTimes.push_back( pNodeAnim->mPositionKeys[k].mTime );
				abn.positions.push_back( glm::vec3( pNodeAnim->mPositionKeys[k].mValue.x, pNodeAnim->mPositionKeys[k].mValue.y, pNodeAnim->mPositionKeys[k].mValue.z ) );
			}

			for (uint32 k = 0; k < pNodeAnim->mNumRotationKeys; ++k)
			{
				const auto& rk = pNodeAnim->mRotationKeys[k];

				abn.rotationTimes.push_back( rk.mTime );

				glm::quat rotation = glm::quat( rk.mValue.w, rk.mValue.x, rk.mValue.y, rk.mValue.z );
				rotation = glm::normalize( rotation );
				abn.rotations.push_back( rotation );
			}

			for (uint32 k = 0; k < pNodeAnim->mNumScalingKeys; ++k)
			{
				abn.scalingTimes.push_back( pNodeAnim->mScalingKeys[k].mTime );
				abn.scalings.push_back( glm::vec3( pNodeAnim->mScalingKeys[k].mValue.x, pNodeAnim->mScalingKeys[k].mValue.y, pNodeAnim->mScalingKeys[k].mValue.z ) );
			}

			// Add AnimatedBoneNode to the animation
			if( animatedBoneNodes_.find( abn.name ) == animatedBoneNodes_.end() )
			{
				animatedBoneNodes_[ abn.name ] = std::move(abn);
			}
			else
			{
				// Warning - animated bone node already exists!
                LOG_WARN(logger, "Animated bone node with name '%s' already exists for mesh '%s' for model '%s'!" , abn.name, filename, name);
			}
		}

        LOG_DEBUG(logger, "Done importing animation with name '%s' for mesh '%s' for model '%s'." , name_, filename, name);
	}
};

}

#endif /* ANIMATION_H_ */
