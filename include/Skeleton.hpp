#ifndef SKELETON_H_
#define SKELETON_H_

#include <vector>
#include <map>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "logger/ILogger.hpp"
#include "fs/IFileSystem.hpp"

#include "Types.hpp"

struct aiScene;
struct aiNode;

namespace ice_engine
{

struct BoneNode
{
	std::string name;
	glm::mat4 transformation = glm::mat4(1.0f);
	std::vector<BoneNode> children;
};

class Skeleton
{
public:

	Skeleton() = default;
	
	Skeleton(
		std::string name,
		BoneNode rootBoneNode,
		glm::mat4 globalInverseTransformation
	)
	:
		name_(std::move(name)),
		rootBoneNode_(std::move(rootBoneNode)),
		globalInverseTransformation_(std::move(globalInverseTransformation))
	{
	}

	Skeleton(const std::string& name, const std::string& filename, const aiScene* scene, logger::ILogger* logger, fs::IFileSystem* fileSystem)
	{
		import(name, filename, scene, logger, fileSystem);
	}

	const std::string& name() const
	{
		return name_;
	}

	const BoneNode& rootBoneNode() const
	{
		return rootBoneNode_;
	}

	const glm::mat4& globalInverseTransformation() const
	{
		return globalInverseTransformation_;
	}

private:
	std::string name_;
	BoneNode rootBoneNode_;
	glm::mat4 globalInverseTransformation_ = glm::mat4(1.0f);

	BoneNode importBoneNode(const aiNode* node);

	void import(const std::string& name, const std::string& filename, const aiScene* scene, logger::ILogger* logger, fs::IFileSystem* fileSystem);
};

}

#endif /* SKELETON_H_ */
