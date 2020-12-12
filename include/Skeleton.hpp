#ifndef SKELETON_H_
#define SKELETON_H_

#include <vector>
#include <map>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <assimp/scene.h>

#include "logger/ILogger.hpp"
#include "fs/IFileSystem.hpp"

#include "detail/AssImpUtilities.hpp"

#include "Types.hpp"

namespace ice_engine
{

struct BoneNode
{
	std::string name;
	glm::mat4 transformation;
	std::vector< BoneNode > children;
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
	glm::mat4 globalInverseTransformation_;

	BoneNode importBoneNode(const aiNode* node)
	{
		BoneNode boneNode = BoneNode();
		boneNode.name = std::string( node->mName.C_Str() );
		boneNode.transformation = detail::convertAssImpMatrix( &(node->mTransformation) );

		for (uint32 i = 0; i < node->mNumChildren; i++)
		{
			boneNode.children.push_back( importBoneNode( node->mChildren[i] ) );
		}

		return boneNode;
	}

	void import(const std::string& name, const std::string& filename, const aiScene* scene, logger::ILogger* logger, fs::IFileSystem* fileSystem)
	{
		assert(scene != nullptr);

        LOG_DEBUG(logger, "Importing skeleton for mesh '%s' for model '%s'." , filename, name);

		globalInverseTransformation_ = glm::inverse( detail::convertAssImpMatrix( &(scene->mRootNode->mTransformation) ) );

		// Load BoneNodes
		const aiNode* assImpRootNode = scene->mRootNode;

		rootBoneNode_ = importBoneNode( assImpRootNode );

        LOG_DEBUG(logger, "Done importing skeleton for mesh '%s' for model '%s'." , filename, name);
	}
};

}

#endif /* SKELETON_H_ */
