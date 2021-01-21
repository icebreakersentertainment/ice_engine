#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include <assimp/anim.h>

#include "Skeleton.hpp"

#include "detail/AssImpUtilities.hpp"
#include "detail/Assert.hpp"

namespace ice_engine
{

BoneNode Skeleton::importBoneNode(const aiNode* node)
{
    BoneNode boneNode = BoneNode();
    boneNode.name = std::string(node->mName.C_Str());
    boneNode.transformation = detail::toGlm(node->mTransformation);

    for (uint32 i = 0; i < node->mNumChildren; ++i)
    {
        boneNode.children.push_back(importBoneNode(node->mChildren[i]));
    }

    return boneNode;
}

void Skeleton::import(const std::string& name, const std::string& filename, const aiScene* scene, logger::ILogger* logger, fs::IFileSystem* fileSystem)
{
    ICE_ENGINE_ASSERT(scene != nullptr);

    LOG_DEBUG(logger, "Importing skeleton for mesh '%s' for model '%s'." , filename, name);

    globalInverseTransformation_ = glm::inverse(detail::toGlm(scene->mRootNode->mTransformation));

    // Load BoneNodes
    const aiNode* assImpRootNode = scene->mRootNode;

    rootBoneNode_ = importBoneNode(assImpRootNode);

    LOG_DEBUG(logger, "Done importing skeleton for mesh '%s' for model '%s'." , filename, name);
}

}
