#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <vector>
#include <unordered_map>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Skeleton.hpp"

#include "logger/ILogger.hpp"
#include "fs/IFileSystem.hpp"

#include "Types.hpp"

struct aiAnimation;

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

	void import(const std::string& name, const std::string& filename, const aiAnimation* animation, logger::ILogger* logger, fs::IFileSystem* fileSystem);
};

}

#endif /* ANIMATION_H_ */
