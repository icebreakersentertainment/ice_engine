#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include <chrono>

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

template <typename T>
struct KeyFrame
{
    KeyFrame() = default;

    KeyFrame(std::chrono::duration<float32> time, T transformation) : time(std::move(time)), transformation(std::move(transformation))
    {}

    std::chrono::duration<float32> time{0.0f};
    T transformation;
};

struct AnimatedBoneNode
{
	AnimatedBoneNode() = default;

	AnimatedBoneNode(
		std::string name,
        std::vector<KeyFrame<glm::vec3>> positionKeyFrames,
        std::vector<KeyFrame<glm::quat>> rotationKeyFrames,
        std::vector<KeyFrame<glm::vec3>> scalingKeyFrames
	) : name(std::move(name)), positionKeyFrames(std::move(positionKeyFrames)), rotationKeyFrames(std::move(rotationKeyFrames)), scalingKeyFrames(std::move(scalingKeyFrames))
	{
	}

	std::string name;

	std::vector<KeyFrame<glm::vec3>> positionKeyFrames;
	std::vector<KeyFrame<glm::quat>> rotationKeyFrames;
	std::vector<KeyFrame<glm::vec3>> scalingKeyFrames;
};

class Animation
{
public:

	Animation() = default;

	Animation(
		std::string name,
        std::chrono::duration<float32> duration,
		float32 ticksPerSecond,
		std::unordered_map<std::string, AnimatedBoneNode> animatedBoneNodes
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

    std::chrono::duration<float32> duration() const
	{
		return duration_;
	}

	float32 ticksPerSecond() const
	{
		return ticksPerSecond_;
	}

	const std::unordered_map<std::string, AnimatedBoneNode>& animatedBoneNodes() const
	{
		return animatedBoneNodes_;
	}

private:
	std::string name_;
    std::chrono::duration<float32> duration_ = std::chrono::duration<float32>(0.0f);
	float32 ticksPerSecond_ = 0.0f;
	std::unordered_map< std::string, AnimatedBoneNode > animatedBoneNodes_;

	void import(const std::string& name, const std::string& filename, const aiAnimation* animation, logger::ILogger* logger, fs::IFileSystem* fileSystem);
};

}

#endif /* ANIMATION_H_ */
